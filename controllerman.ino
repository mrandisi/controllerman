/**
 * ControllerMan
 * www.controllerman.com
 * 
 */
#include <Bounce2.h>      // buttons anti bounce
#include <avr/pgmspace.h> // Provides PROGMEM to read directly from the ROM
#include <MIDI.h>
#include "DeviceSettings.h"
#include "Display.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI); //Serial1
//MIDI_CREATE_DEFAULT_INSTANCE();

const uint8_t BUTTQTY = 6;

// pinout configuration
const uint8_t PIN_BUTTON [BUTTQTY] = {14, 15, 16, 17, 18, 19}; // from 1st to 6th
const uint8_t PIN_RGB_RED = 5;
const uint8_t PIN_RGB_GREEN = 6;
const uint8_t PIN_RGB_BLUE = 9;

// Define possible button combinations
// First {2,4} means that button 1 can be combined as 1+2 and 1+4
const uint8_t DOUBLE_BUTT_COMBINATION[6][3] = {
          {2,4},    // butt 1
          {1,3,5},  // butt 2
          {2,6},    // butt 3
          {1,5},    // butt 4
          {4,2,6},  // butt 5
          {3,5}};   // butt 6

// gree, cyan, red, purple
const uint8_t LAYOUT_COLOR[4][3] = { {255,0,0}, {0,128,255}, {0,255,0}, {255,0,255} }; 
//const char * LAYOUT_NAME[4] = {"Red", "Cyan", "Green", "Purple"};

bool TITLE_TIMEOUT_ENABLED=false;
int TITLE_START_TIME = 0;
bool EDIT_MODE=false;

// Instantiate the Bounce objects
Bounce * DEBOUNCER = new Bounce[BUTTQTY];   // 6 debouncing objects

void setup() {
  init_display();
  
  //MIDI.begin();
  MIDI.begin(MIDI_CHANNEL_OMNI); // Initialize the Midi Library.
  MIDI.setHandleControlChange(processCCmsg); // This command tells the MIDI Library
  MIDI.setHandleProgramChange(processPCmsg); // This command tells the MIDI Library
  
  // Initialize the button pins
  for (int i = 0; i < BUTTQTY; i++) {
    pinMode(PIN_BUTTON[i] ,INPUT_PULLUP);
  }

  // Initialize RGB led pins
  pinMode(PIN_RGB_RED, OUTPUT);
  pinMode(PIN_RGB_GREEN, OUTPUT);
  pinMode(PIN_RGB_BLUE, OUTPUT);

  // Initialize the bounce objects
  int bounceInterval = 5; // Millisec.
  for (int i = 0; i < BUTTQTY; i++) {
    DEBOUNCER[i].attach(PIN_BUTTON[i]);
    DEBOUNCER[i].interval(bounceInterval);
  }
  
  //splash();
  //ledTest();
  //delay(1000);

  load_default_states();
  loadLayout();
  
  // Synchronize all data at the beginning
  updateDevice();
} // end Setup

void loop() {

  //MIDI.read(); // Continuously check if Midi data has been received.
  
  // Check all buttons
  for(uint8_t i=0; i<BUTTQTY; i++) {
    DEBOUNCER[i].update();
    if(DEBOUNCER[i].read()==LOW) { // button is pressed
      
      // Stops checking all the hardware focusing only on one control
      manageAction(i);
    }
  } // end for all buttons cycle
  
  // title timeout restore
  if(TITLE_TIMEOUT_ENABLED) {
    int nowTime = millis();
    if(nowTime-TITLE_START_TIME > 1000) {
      sprintf (scr.title, "P%03d", PATCH_STATE);
      drawLayout(scr);
      TITLE_TIMEOUT_ENABLED=false;
    }
  }
  
  if(EDIT_MODE) {
    settings_menu();
    EDIT_MODE=false;
    drawLayout(scr);
  }
  
  // TODO: Lazy Ops.
  
} // end loop()


/**----------------------------------------------------------
 *          MIDI READ
 ----------------------------------------------------------*/
void processPCmsg(byte channel, byte number, byte value) {
  //PATCH_STATE=number+1;
  char buf[8];
  sprintf (buf, "PC %d", number);
  setTemporaryTitle(buf);
}
void processCCmsg(byte channel, byte number, byte value) {
  char buf[8];
  sprintf (buf, "CC %d", number);
  setTemporaryTitle(buf);
}
/**----------------------------------------------------------
 *          END MIDI READ 
 * ----------------------------------------------------------*/





 /**----------------------------------------------------------
 *          BUTTON MNGMNT
 * ----------------------------------------------------------*/
 
/**
 * A delay function that refresh processes while waits
 */
void customDelay(int delayTime) {
  const int startTime = millis();
  int nowTime=startTime;
  while(nowTime-startTime < delayTime) {

    updateRealtimeProcesses();
    
    nowTime = millis();
  }
}
void updateRealtimeProcesses() {
  //MIDI.read(); // Continuously check if Midi data has been received.
}

void manageAction(uint8_t i) {
  unsigned long press_time = millis();
  
  while(DEBOUNCER[i].read()==LOW) { // while button is released
    
    if((millis() - press_time) < 1000) {  // not a long press. User haves one second to press also a second button)
      
      // check nearby buttons
      uint8_t j=0;
      while(j<3 && DOUBLE_BUTT_COMBINATION[i][j] != 0) {  // try all combintations
  
        uint8_t nearButton = DOUBLE_BUTT_COMBINATION[i][j];
        
        DEBOUNCER[nearButton-1].update();
  
        if(DEBOUNCER[nearButton-1].read()==LOW) { // second button is pressed
          unsigned long double_press_time = millis();
          
          // wait for release of both buttons
          while(DEBOUNCER[i].read()==LOW || DEBOUNCER[nearButton-1].read()==LOW) {
  
            // Double long press
            if((millis() - double_press_time) > 1000) {
              // Run double button long press action
              bool break_flag = doublePress_hold(i+1, nearButton);
              
              if(break_flag) {  // wait for buttons release
                while(DEBOUNCER[i].read()==LOW || DEBOUNCER[nearButton-1].read()==LOW) {
                  DEBOUNCER[i].update();
                  DEBOUNCER[nearButton-1].update();
                }
              } // end if wait
              
            }
            DEBOUNCER[i].update();
            DEBOUNCER[nearButton-1].update();
          } // end while long press
          
          if((millis() - double_press_time) <= 1000) {  // avoid single action after long press
            // RUN DOUBLE BUTTON i,j ACTION FOR CURRENT LAYOUT
            doublePress(i+1, nearButton);
          }
          return;
        } // end if second button pressed
        
        j++;
      } // end try all combinations
      
    } else {  // Long press detected
      // System continues to call this field
      // while button is released or action returns break=true;
      bool break_flag = singlePress_hold(i+1);

      if(break_flag) {  // wait for button release
        while(DEBOUNCER[i].read()==LOW) {
          DEBOUNCER[i].update();
        }
      }
    }
    
    DEBOUNCER[i].update();  // update the state of the pressed button
  }
  // primary button released (without double button combination)
  if((millis() - press_time) < 1000) {
    
    //  RUN SIMPLE ACTION
    singlePress(i+1);
  }
  
} // end manage action

void singlePress(uint8_t button) {

  scr.buttonState[button-1]=!scr.buttonState[button-1];
  drawLayout(scr);
  
  uint8_t virtual_button_index = (6 * CURRENT_LAYOUT) + (button-1);
  //char sn[4];
  //getFxShortName(virtual_button_index, sn);
  char ln[11];
  getFxLongName(virtual_button_index, ln);
  
  if(button_states[virtual_button_index]==false) {  // is inactive
    button_states[virtual_button_index]=true;
    //MIDI.sendControlChange(ccs[virtual_button_index], 127, channel); // activate
    MIDI.sendControlChange(getFxChannel(virtual_button_index), 127, channel); // activate
    char buf[16];
    sprintf(buf, "%s ON", ln);
    setTemporaryTitle(buf);
  } else {                    // is active
    button_states[virtual_button_index]=false;
    //MIDI.sendControlChange(ccs[virtual_button_index], 0, channel);
    MIDI.sendControlChange(getFxChannel(virtual_button_index), 0, channel);
    char buf[16];
    sprintf(buf, "%s OFF", ln);
    setTemporaryTitle(buf);
  } // end if false

  // TODO: synchronize the state with any other occourcences of the same effect

  loadLayout();
}

bool singlePress_hold(uint8_t button) {
  //bool break_flag = LAYOUT->singlePress_hold(button);
  return false;//break_flag;
}

void doublePress(uint8_t button1, uint8_t button2) {
  // sort the button numbers for convenience
  if(button1 > button2) { 
    uint8_t temp = button1;
    button1 = button2;
    button2 = temp;
  }
  
  if(button1==3 && button2==6) {  // combination reserved to scroll the layout forward
    scroll_next_layout();
  } else if(button1==1 && button2==4) {  // combination reserved to scroll the layout forward
    scroll_prev_layout();
  } else if(button1==2 && button2==5) {
    reset_layout();
  } else if(button1==5 && button2==6) {  // combination reserved to patch up/down
    patchUp();
  } else if(button1==2 && button2==3) {
    patchDown();
  }
  
}

bool doublePress_hold(uint8_t button1, uint8_t button2) {
  // sort the button numbers for convenience
  if(button1 > button2) { 
    uint8_t temp = button1;
    button1 = button2;
    button2 = temp;
  }
  
  if(button1==3 && button2==6) {  // combination reserved to scroll the layout forward
    scroll_next_layout();
    delay(100);
    return false;
  } else if(button1==1 && button2==4) {  // combination reserved to scroll the layout forward
    scroll_prev_layout();
    delay(100);
    return false;
  } else if(button1==2 && button2==5) {
    EDIT_MODE=true;
    return true;
  }
  
  return true;//break_flag, true waits for button release;
}

 /**----------------------------------------------------------
 *          END BUTTON MNGMNT
 * ----------------------------------------------------------*/



/** ----------------------------------------------------------
 *          UTILS
 * ----------------------------------------------------------*/
 
void scroll_next_layout() {
  if(CURRENT_LAYOUT < 3) {
    CURRENT_LAYOUT++;
  } else {
    CURRENT_LAYOUT=0;
  }
  loadLayout();
  setTemporaryTitle(LAYOUT_TITLE);
}

void scroll_prev_layout() {
  if(CURRENT_LAYOUT > 0) {
    CURRENT_LAYOUT--;
  } else {
    CURRENT_LAYOUT=3;
  }
  loadLayout();
  setTemporaryTitle(LAYOUT_TITLE);
}

void reset_layout() {
  CURRENT_LAYOUT=0;
  loadLayout();
  setTemporaryTitle(LAYOUT_TITLE);
}

void loadLayout() {
  char lName[14];
  const char lStr[8] = " layout";
  strcpy_P(lName, (char*)pgm_read_word(&( layoutName[CURRENT_LAYOUT] )));
  
  setRgbColor(LAYOUT_COLOR[CURRENT_LAYOUT]);
  strcat(lName, lStr);
  strcpy(LAYOUT_TITLE, lName);
  
  // load button states from current layout 
  uint8_t shift = 6 * CURRENT_LAYOUT;  // 6*0=0; 6*1=6; 6*2=12...
  for(uint8_t i=0; i<6; i++) {
    scr.buttonState[i] = button_states[i+shift];
  }
  loadFxNames();
  drawLayout(scr);
}

void loadFxNames() {
  uint8_t shift = 6*CURRENT_LAYOUT;  // 6*0=0; 6*1=6; 6*2=12...
  
  for(uint8_t i=0; i<6; i++) {
    
    getFxShortName(i+shift, scr.fxLabel[i]);
    getFxLongName(i+shift, scr.fxDescr[i]);
  }
}

void patchUp(){
  if(PATCH_STATE < 125) { // 125 is the last patch, 126 and 127 are not used.
    PATCH_STATE++;
  } else {
    PATCH_STATE=1;  // loops to the first patch
  }
  sprintf (scr.title, "P%03d", PATCH_STATE);
  load_default_states();
  loadLayout();
  MIDI.sendProgramChange (PATCH_STATE-1, channel); // counts from 0 that stands for the patch 1
  delay(5);
}

void patchDown() {
  if(PATCH_STATE > 1) {
    PATCH_STATE--;
  } else {
    PATCH_STATE=125;  // loops to the last patch
  }
  sprintf (scr.title, "P%03d", PATCH_STATE);
  load_default_states();
  loadLayout();
  MIDI.sendProgramChange (PATCH_STATE-1, channel); // counts from 0 that stands for the patch 1
  delay(5);
}

void updateDevice() {
  // send the patch number
  MIDI.sendProgramChange (PATCH_STATE-1, channel); // counts from 0 that stands for the patch 1

  // TODO: foreach button
  //          --> send the state
}

void setRgbColor(uint8_t c[]) {
  uint8_t divider = 64;
  analogWrite(PIN_RGB_RED, c[0]/divider);
  analogWrite(PIN_RGB_GREEN, c[1]/divider);
  analogWrite(PIN_RGB_BLUE, c[2]/divider);
}

void ledTest() {

  // RGB animation
  int rgbWait=10000;
  /*for(int i=0; i<256; i++){
    analogWrite(PIN_RGB_RED, i);
    delayMicroseconds(rgbWait);
  }
    for(int i=255; i>=0; i--){
    analogWrite(PIN_RGB_RED, i);
    delayMicroseconds(rgbWait);
  }
  for(int i=0; i<256; i++){
    analogWrite(PIN_RGB_GREEN, i);
    delayMicroseconds(rgbWait);
  }
    for(int i=255; i>=0; i--){
    analogWrite(PIN_RGB_GREEN, i);
    delayMicroseconds(rgbWait);
  }
  for(int i=0; i<256; i++){
    analogWrite(PIN_RGB_BLUE, i);
    delayMicroseconds(rgbWait);
  }
  for(int i=255; i>=0; i--){
    analogWrite(PIN_RGB_BLUE, i);
    delayMicroseconds(rgbWait);
  }*/

  for(int i=0; i<64; i++){
    analogWrite(PIN_RGB_RED, i);
    //analogWrite(PIN_RGB_GREEN, i);
    //analogWrite(PIN_RGB_BLUE, i);
    delayMicroseconds(rgbWait);
  }
  for(int i=64; i>=0; i--){
    analogWrite(PIN_RGB_RED, i);
    //analogWrite(PIN_RGB_GREEN, i);
    //analogWrite(PIN_RGB_BLUE, i);
    delayMicroseconds(rgbWait);
  }
} // end led test

void setTemporaryTitle(char * tmpTitle) {
  // TODO draw frame
  TITLE_TIMEOUT_ENABLED=true;
  TITLE_START_TIME = millis();
  strcpy(scr.title,tmpTitle);
  drawLayout(scr);
}

/** ----------------------------------------------------------
 *          END UTILS
 * ----------------------------------------------------------*/



/** ----------------------------------------------------------
 *          SETTINGS MENU
 * ----------------------------------------------------------*/


void(* reboot)(void) = 0;

uint8_t checkButtons() {
  uint8_t btChoose=0;
  while(btChoose == 0) {
      
    for(uint8_t i=0; i<6; i++) {
      DEBOUNCER[i].update();
      if(DEBOUNCER[i].read()==LOW) { // button is pressed
        
        // waits for release
        while(DEBOUNCER[i].read()==LOW) {
          DEBOUNCER[i].update();
        }
        btChoose = i+1;
        return btChoose;
      }
    } // end for all buttons cycle
  } // end while
  return 0;
}

uint8_t FROM_LAYOUT;
uint8_t FROM_BUTTON;
uint8_t TO_LAYOUT;
uint8_t TO_BUTTON;

void settings_menu() {
  EDIT_MODE=true;

  uint8_t m_index=0;
  char title[20] = "Settings";
  char * m_options[20] = {"Swap Buttons","Save button states","Edit button","Set the boot patch","Calibrate exp pedal","Reset device"};
  uint8_t m_option_len = 6;
  uint8_t m_selected_entry=0;
  bool m_buttState[6]={1,1,1,0,1,1};
  uint8_t go_fw=0;
  
  go_fw = menu_navigation(m_index, title, m_options, m_option_len, m_selected_entry, m_buttState);

  if(go_fw==1) { // choice swap
    strcpy(title, "From layout");
    char  * m_options[20] = {"Red", "Cyan", "Green", "Purple"};
    m_option_len = 4;
    go_fw = menu_navigation(m_index++, title, m_options, m_option_len, m_selected_entry, m_buttState);
    if(go_fw>0) {  // choice FROM_BUTTON
      FROM_LAYOUT=go_fw;
      strcpy(title, "From button");
      
      char * m_options[20] = {"1", "2", "3", "4", "5", "6"};
      
      m_option_len = 6;
      go_fw = menu_navigation(m_index++, title, m_options, m_option_len, m_selected_entry, m_buttState);

      if(go_fw>0) {  // choose TO_LAYOUT
        FROM_BUTTON=go_fw;
        strcpy(title, "To layout");
        char * m_options[20] = {"Red", "Cyan", "Green", "Purple"};
        m_option_len = 4;
        go_fw = menu_navigation(m_index++, title, m_options, m_option_len, m_selected_entry, m_buttState);
        if(go_fw>0) {
          TO_LAYOUT=go_fw;
          strcpy(title, "To button");
          char * m_options[20] = {"1", "2", "3", "4", "5", "6"};
          m_option_len = 6;
          go_fw = menu_navigation(m_index++, title, m_options, m_option_len, m_selected_entry, m_buttState);
          if(go_fw>0) {  // choose TO_BUTTON
            TO_BUTTON=go_fw;
            
            // SWAP BUTTONS
            
            uint8_t fromVButton = (FROM_BUTTON-1) + (6*(FROM_LAYOUT-1)); // from 0 to 23
            uint8_t toVButton = (TO_BUTTON-1) + (6* (TO_LAYOUT-1)); // from 0 to 23

            // metto temporaneamente TO in tmp
            byte tmpChannel = getFxChannel(toVButton);
            char tmpShortName[3];
            getFxShortName(toVButton, tmpShortName);
            char tmpLongName[10];
            getFxLongName(toVButton, tmpLongName);
            
            // metto FROM in TO
            byte fromChannel = getFxChannel(fromVButton);
            char fromShortName[4];
            getFxShortName(fromVButton, fromShortName);
            char fromLongName[11];
            getFxLongName(fromVButton, fromLongName);
            
            setFxChannel(toVButton, fromChannel);
            writeFxShortName(toVButton, fromShortName);
            setFxLongName(toVButton, fromLongName);

            // metto tmp in FROM
            setFxChannel(fromVButton, tmpChannel);
            writeFxShortName(fromVButton, tmpShortName);
            setFxLongName(fromVButton, tmpLongName);
            
            //load_default_states();
            loadLayout();

          } else {
            return;
          }
        } else {
          return;
        }
      } else {
        return;
      }
    } else {
      return;
    }
  } else if(go_fw==2) {
    setTemporaryTitle("Save fx states");
    write_button_states();
  } else if(go_fw==3) {
    setTemporaryTitle("Not yet implemented");
  } else if(go_fw==4) {
    setTemporaryTitle("Not yet implemented");
  } else if(go_fw==5) {
    setTemporaryTitle("Not yet implemented");
  } else if(go_fw==6) {
    setTemporaryTitle("Reset, wait...");
    clear_eeprom();
    write_default_fx();
    reboot();
  } else {
    return;
  }
    
  
}



// gestisce lo spostamento interno del menu, ritorna 0 o 1 per tornare indietro o andare avanti
uint8_t menu_navigation(uint8_t m_index, char title[], char * m_options[], uint8_t m_option_len, uint8_t m_selected_entry, bool m_buttState[]) {
  
  while(true) {
    
    drawMenu(title, m_options, m_option_len, m_selected_entry, m_buttState);
    
    uint8_t butt_choice = checkButtons();  // waits for a choose

    
    if(butt_choice==5) {       // go up
      
      m_selected_entry = m_selected_entry<1 ? m_option_len-1 : m_selected_entry-1;
    }else if(butt_choice==2) { // go down
      
      m_selected_entry = m_selected_entry>=m_option_len-1 ? 0 : m_selected_entry+1;
    } else if(butt_choice==1 || butt_choice==6) {
      return m_selected_entry+1;
    } else{
      break;
    }
  }
  
  return 0;
}

/** ----------------------------------------------------------
 *          END SETTINGS MENU
 * ----------------------------------------------------------*/
