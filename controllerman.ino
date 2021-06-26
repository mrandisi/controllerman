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
unsigned long TITLE_START_TIME = 0;
bool EDIT_MODE=false;
/*unsigned long TAP_TIME=0;
unsigned long TAP_INTERVAL=0;
unsigned long LAST_TAP_BLINK=millis();*/
unsigned long LAZY_TIME=millis();

// Instantiate the Bounce objects
Bounce * DEBOUNCER = new Bounce[BUTTQTY];   // 6 debouncing objects

void setup() {
  init_display();
  
  //MIDI.begin();
  MIDI.begin(MIDI_CHANNEL_OMNI); // Initialize the Midi Library.
  //MIDI.setHandleControlChange(processCCmsg); // This command tells the MIDI Library
  //MIDI.setHandleProgramChange(processPCmsg); // This command tells the MIDI Library
  
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
  
  // boot animation
  splash();
  ledTest();
  delay(500);

  PATCH_STATE = read_default_patch();
  sprintf (scr.title, "P%03d", PATCH_STATE);
  load_default_states(PATCH_STATE);
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
  
  // Lazy Ops.
  if(LAZY_TIME-millis()>10) { // every 10ms
    LAZY_TIME=millis();

    
    //int msInterval = TAP_INTERVAL*100/6;
    /*if(TAP_INTERVAL>=100 && millis()-LAST_TAP_BLINK > TAP_INTERVAL) {
      tapBlink();
      LAST_TAP_BLINK=millis();
    }*/

    // opens the menu
    if(EDIT_MODE) {
      settings_menu();
      EDIT_MODE=false;
      drawLayout(scr);
    }

    // title timeout restore
    if(TITLE_TIMEOUT_ENABLED) {
      unsigned long nowTime = millis();
      if(nowTime-TITLE_START_TIME > 1000) {
        sprintf (scr.title, "P%03d", PATCH_STATE);
        drawLayout(scr);
        TITLE_TIMEOUT_ENABLED=false;
      }
    }

  }
  
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
  char sn[4];
  getFxShortName(virtual_button_index, sn);
  char ln[11];
  getFxLongName(virtual_button_index, ln);
  uint8_t ccnum = getFxChannel(virtual_button_index);

  // hook special function
  /*if(!strcmp(sn, "TAP")) {
    
    if(TAP_TIME==0) {
      TAP_TIME=millis();
    } else {
      TAP_INTERVAL = millis() - TAP_TIME; // *60 sec /1000ms
      
      //if(tap > 60 && tap < 250) {
        char buf[16];
        sprintf(buf, "TAP %dms", (TAP_INTERVAL*6)/100);
        setTemporaryTitle(buf);
      //}
      TAP_TIME=millis();
    }
    tapBlink();
  } 
  else */
  if(button_states[virtual_button_index]==false) {  // is inactive
    button_states[virtual_button_index]=true;
    MIDI.sendControlChange(getFxChannel(virtual_button_index), 127, channel); // activate
    char buf[16];
    sprintf(buf, "%s %d ON", ln, ccnum);
    setTemporaryTitle(buf);
  } else {                    // is active
    button_states[virtual_button_index]=false;
    MIDI.sendControlChange(getFxChannel(virtual_button_index), 0, channel);
    char buf[16];
    sprintf(buf, "%s %d OFF", ln, ccnum);
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
  } else if(button1==4 && button2==5) {  // jump patch up/down
    jumpPatchUp();
  } else if(button1==1 && button2==2) {
    jumpPatchDown();
  }
  
}

bool doublePress_hold(uint8_t button1, uint8_t button2) {
  // sort the button numbers for convenience
  if(button1 > button2) { 
    uint8_t temp = button1;
    button1 = button2;
    button2 = temp;
  }
  
  if(button1==2 && button2==5) {
    EDIT_MODE=true;
    return true;
  } else if(button1==5 && button2==6) {  // combination reserved to patch up/down
    patchUp();
    //delay(10);
    return false;
  } else if(button1==2 && button2==3) {
    patchDown();
    //delay(10);
    return false;
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
  load_default_states(PATCH_STATE);
  loadLayout();
  updateDevice();
}

void patchDown() {
  if(PATCH_STATE > 1) {
    PATCH_STATE--;
  } else {
    PATCH_STATE=125;  // loops to the last patch
  }
  sprintf (scr.title, "P%03d", PATCH_STATE);
  load_default_states(PATCH_STATE);
  loadLayout();
  updateDevice();
}

void jumpPatchUp(){
  if(PATCH_STATE < 115) { // 125 is the last patch, 126 and 127 are not used.
    PATCH_STATE+=10;
  } else {
    PATCH_STATE-=113;  // loops to the first patch
  }
  sprintf (scr.title, "P%03d", PATCH_STATE);
  load_default_states(PATCH_STATE);
  loadLayout();
  updateDevice();
}

void jumpPatchDown() {
  if(PATCH_STATE >= 11) {
    PATCH_STATE-=10;
  } else {
    PATCH_STATE+=123;  // loops to the last patch
  }
  sprintf (scr.title, "P%03d", PATCH_STATE);
  load_default_states(PATCH_STATE);
  loadLayout();
  updateDevice();
}

void updateDevice() {
  // send the patch number
  MIDI.sendProgramChange (PATCH_STATE-1, channel); // counts from 0 that stands for the patch 1
  delay(8);
  for(uint8_t i=0; i<24; i++) {
      if(button_states[i]==false) {
        MIDI.sendControlChange(getFxChannel(i), 0, channel);
      } else {
        MIDI.sendControlChange(getFxChannel(i), 127, channel);
      }
      delay(8);
  }
}

void setRgbColor(uint8_t c[]) {
  uint8_t divider = 64;
  analogWrite(PIN_RGB_RED, c[0]/divider);
  analogWrite(PIN_RGB_GREEN, c[1]/divider);
  analogWrite(PIN_RGB_BLUE, c[2]/divider);
}



void setTemporaryTitle(char * tmpTitle) {
  // TODO draw frame
  TITLE_TIMEOUT_ENABLED=true;
  TITLE_START_TIME = millis();
  strcpy(scr.title,tmpTitle);
  drawLayout(scr);
}

void ledTest() {
  // RGB animation
  //int rgbWait=500;
  for(int i=0; i<64; i++){ // Red
    analogWrite(PIN_RGB_RED, i);
    //delayMicroseconds(rgbWait);
    delay(10);
  }
  for(int i=64; i>=0; i--){
    analogWrite(PIN_RGB_RED, i);
    //delayMicroseconds(rgbWait);
    delay(10);
  }
} // end led test

void tapBlink() {
  analogWrite(PIN_RGB_RED, 255);
  analogWrite(PIN_RGB_GREEN, 255);
  analogWrite(PIN_RGB_BLUE, 255);
  delay(3);
  setRgbColor(LAYOUT_COLOR[CURRENT_LAYOUT]);
} // end led Blink

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
        
        // hook waiting for release
        while(DEBOUNCER[i].read()==LOW) {
          DEBOUNCER[i].update();
        }

        btChoose = i+1; // button number
        return btChoose;
      }
    } // end for all buttons cycle
  } // end while
  return 0;
}

void settings_menu() {
  EDIT_MODE=true;

  uint8_t m_index=0;
  char title[20] = "Settings";
  char * m_options[20] = {"Save button states","Swap Buttons","Edit button","Set as boot patch","Reset device"};
  uint8_t m_option_len = 5;
  uint8_t m_selected_entry=0;
  bool m_buttState[6]={1,1,1,0,1,1};
  uint8_t go_fw=0;
  
  go_fw = menu_navigation(m_index, title, m_options, m_option_len, m_selected_entry, m_buttState);

  if(go_fw==1) { // save button states
    setTemporaryTitle("Save fx states");
    write_default_states(PATCH_STATE);

  } else if(go_fw==2) { // swap
            
    uint8_t fromVButton = chooseButton("Move From");
    if(!fromVButton) return;

    uint8_t toVButton = chooseButton("Move To");
    if(!toVButton) return;
    
    swapButtons(fromVButton, toVButton);
    swapStates(fromVButton, toVButton);
    loadLayout();
    
    setTemporaryTitle("Swap done!");
    
  } else if(go_fw==3) { // Edit button
    

    uint8_t virtual_button = 0;
    virtual_button = chooseButton("Button to edit");
    
    if(!virtual_button) return;

    if(editButton(virtual_button)) {
      setTemporaryTitle("Saved!");
      loadLayout();
    }

  } else if(go_fw==4) { // Save patch
    setTemporaryTitle("Saved!");
    write_default_patch(PATCH_STATE);

  } else if(go_fw==5) { // Reset
    setTemporaryTitle("Reset, wait...");
    clear_eeprom();
    write_default_fx();
    reboot();

  }
  
}

uint8_t chooseButton(char* title) { // return 1-24 or 0 as no choice
  
  bool m_buttState[6]={1,1,1,0,1,1};
  
  char  * m_options1[20] = {"Red", "Cyan", "Green", "Purple"};
  uint8_t m_option_len = 4;
  uint8_t fromLayout = menu_navigation(0, title, m_options1, m_option_len, 0, m_buttState);
  if(fromLayout <=0) return 0;

  //strcpy(title, "choose button");
  char * m_options2[20] = {"1", "2", "3", "4", "5", "6"};
  m_option_len = 6;
  uint8_t fromButton = menu_navigation(0, title, m_options2, m_option_len, 0, m_buttState);
  if(fromButton <=0) return 0;
  
  uint8_t virtual_button = fromButton + (6*(fromLayout-1)); // from 0 to 23

  return virtual_button;
}

void swapStates(uint8_t fromVButton, uint8_t toVButton) {  // 1 to 24
  fromVButton--;
  toVButton--;

  for(uint8_t i=1; i<=125; i++) {
    load_default_states(i);

    if(button_states[fromVButton] != button_states[toVButton]) {  // if different swap and save
      bool tmp = button_states[fromVButton];
      button_states[fromVButton] = button_states[toVButton];
      button_states[toVButton] = tmp;

      write_default_states(i);
    }
  }
  load_default_states(PATCH_STATE);
}

void swapButtons(uint8_t fromVButton, uint8_t toVButton) {  // 1 to 24
  fromVButton--;
  toVButton--;

  // put TO into tmp
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
  

}

bool editButton(uint8_t virtual_button) { // 1 to 24
  char fx[15];
  getFx(virtual_button-1, fx);

  if(editButton_navigation(fx)) {
    writeFx(virtual_button-1, fx);
    return true;
  }
  return false;
}

uint8_t editButton_navigation(char fx[15]) {
  uint8_t selectedIndex=0;
  
  while(true) {
    drawButtonEdit(selectedIndex, fx);

    uint8_t butt_choice = checkButtons();  // waits for a choose

    if(butt_choice==5) {       // go up
      if(fx[selectedIndex] < 126)
        fx[selectedIndex]+=1;
      else fx[selectedIndex] = 32;

    }else if(butt_choice==2) { // go down
      if(fx[selectedIndex] > 32)
        fx[selectedIndex]-=1;
      else fx[selectedIndex] = 126;
      
    } else if(butt_choice==4) { // go left
      if(selectedIndex>0) {
        selectedIndex--;
      }
    } else if(butt_choice==6) { // go right
      if(selectedIndex<13) {
        selectedIndex++;
      }
    } else if(butt_choice==1) { // return and save changes
      return true;
    } else if(butt_choice==3) { // return and discard changes
      return false;
    }

  }
  
  return 0;
}

// gestisce lo spostamento interno del menu, ritorna 0 o 1 per tornare indietro o andare avanti
uint8_t menu_navigation(uint8_t m_index, char title[], char * m_options[], uint8_t m_option_len, uint8_t m_selected_entry, bool m_buttState[]) {
  uint8_t butt_choice = 0;
  while(butt_choice==0 || butt_choice==5 || butt_choice==2) {
    
    drawMenu(title, m_options, m_option_len, m_selected_entry, m_buttState);
    
    butt_choice = 0;
    butt_choice = checkButtons();  // waits for a choose
    
    if(butt_choice==5) {       // go up
      m_selected_entry = m_selected_entry<1 ? m_option_len-1 : m_selected_entry-1;

    }else if(butt_choice==2) { // go down
      m_selected_entry = m_selected_entry>=m_option_len-1 ? 0 : m_selected_entry+1;

    } else if(butt_choice==1 || butt_choice==6) {
      return m_selected_entry+1;  // confirm
    }

  }
  
  return 0;
}

/** ----------------------------------------------------------
 *          END SETTINGS MENU
 * ----------------------------------------------------------*/