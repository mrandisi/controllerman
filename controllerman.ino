/**
 * ControllerMan
 * www.controllerman.com
 * 
 */
#include <Bounce2.h>  // buttons anti bounce
#include <avr/pgmspace.h> // PROGMEM
#include <MIDI.h>
#include "DeviceSettings.h"
#include "Display.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI); //Serial1
//MIDI_CREATE_DEFAULT_INSTANCE();

const uint8_t ITEMS = 6;

// pinout configuration
const uint8_t PIN_BUTTON [ITEMS] = {14, 15, 16, 17, 18, 19}; // from 1st to 6th
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

uint8_t RGB_STATE[3] = {0,255,0};

bool title_timeout_enabled=false;
int title_start_time = 0;

// Instantiate the Bounce objects
Bounce * DEBOUNCER = new Bounce[ITEMS];   // 6 debouncing objects

void setup() {

  //init_settings();
  init_display();
  
  MIDI.begin();
  //MIDI.begin(MIDI_CHANNEL_OMNI); // Initialize the Midi Library.
  //MIDI.setHandleControlChange(processCCmsg); // This command tells the MIDI Library
  //MIDI.setHandleProgramChange(processPCmsg); // This command tells the MIDI Library
  
  // Initialize the button pins
  for (int i = 0; i < ITEMS; i++) {
    pinMode(PIN_BUTTON[i] ,INPUT_PULLUP);
  }

  // Initialize RGB led pins
  pinMode(PIN_RGB_RED, OUTPUT);
  pinMode(PIN_RGB_GREEN, OUTPUT);
  pinMode(PIN_RGB_BLUE, OUTPUT);

  // Initialize the bounce objects
  int bounceInterval = 5; // Millisec.
  for (int i = 0; i < ITEMS; i++) {
    DEBOUNCER[i].attach(PIN_BUTTON[i]);
    DEBOUNCER[i].interval(bounceInterval);
  }
  
  splash();
  ledTest();
  delay(500);

  load_default_states();
  loadView();
  
  // Synchronize all data at the beginning
  updateDevice();
  
} // end Setup

void loop() {

  //MIDI.read(); // Continuously check if Midi data has been received.
  
  // Check all buttons
  for(uint8_t i=0; i<ITEMS; i++) {
    DEBOUNCER[i].update();
    if(DEBOUNCER[i].read()==LOW) { // button is pressed
      
      // Stops checking all the hardware focusing only on one control
      manageAction(i);
    }
  } // end for all buttons cycle
  
  // title timeout restore
  if(title_timeout_enabled) {
    int nowTime = millis();
    if(nowTime-title_start_time > 1000) {
      sprintf (scr.title, "Patch %d", PATCH_STATE);
      buildScreen();
      title_timeout_enabled=false;
    }
  }

  // TODO: Lazy Ops.
  
} // end loop()

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
              }
            }
            DEBOUNCER[i].update();
            DEBOUNCER[nearButton-1].update();
          } // end while long press
          
          if((millis() - double_press_time) <= 1000) {  // avoid single action after long press
            // RUN DOUBLE BUTTON i,j ACTION FOR CURRENT VIEW
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
  buildScreen();
  
  uint8_t virtual_button_index = (6*CURRENT_VIEW) + (button-1);
  
  if(button_states[virtual_button_index]==false) {  // is inactive
    button_states[virtual_button_index]=true;
    MIDI.sendControlChange(ccs[virtual_button_index], 127, channel); // activate
  } else {                    // is active
    button_states[virtual_button_index]=false;
    MIDI.sendControlChange(ccs[virtual_button_index], 0, channel);
  } // end if false

  loadView();
}

bool singlePress_hold(uint8_t button) {
  //bool break_flag = VIEW->singlePress_hold(button);
  return false;//break_flag;
}

void doublePress(uint8_t button1, uint8_t button2) {
  // sort the button numbers for convenience
  if(button1 > button2) { 
    uint8_t temp = button1;
    button1 = button2;
    button2 = temp;
  }
  //char buf[8];
  //sprintf (buf, "%d", button1+button2);
  //text=buf;
  //buildScreen();
  
  if(button1==3 && button2==6) {  // combination reserved to scroll the view forward
    scroll_next_view();
  } else if(button1==1 && button2==4) {  // combination reserved to scroll the view forward
    scroll_prev_view();
  } else if(button1==2 && button2==5) {
    //reset_view();
  } else if(button1==5 && button2==6) {  // combination reserved to patch up/down
    patchUp();
  } else if(button1==2 && button2==3) {
    patchDown();
  } else {
    //VIEW->doublePress(button1, button2);
  }
}

bool doublePress_hold(uint8_t button1, uint8_t button2) {
  // sort the button numbers for convenience
  if(button1 > button2) { 
    uint8_t temp = button1;
    button1 = button2;
    button2 = temp;
  }
  write_button_states();
  setTemporaryTitle("View saved!");
  //bool break_flag = VIEW->doublePress_hold(button1, button2);
  return true;//break_flag;
}

void scroll_next_view() {
  if(CURRENT_VIEW < 3) {
    CURRENT_VIEW++;
  } else {
    CURRENT_VIEW=0;
  }
  loadView();
}

void scroll_prev_view() {
  if(CURRENT_VIEW > 0) {
    CURRENT_VIEW--;
  } else {
    CURRENT_VIEW=3;
  }
  loadView();
}

void loadView() {
  char ttl[16];
  switch(CURRENT_VIEW) {
    case 0:
      RGB_STATE[0]=0;
      RGB_STATE[1]=255;
      RGB_STATE[2]=0;
      setRgbColor(RGB_STATE);
      strcpy(ttl, "Green View");
      break;
    case 1:
      RGB_STATE[0]=0;
      RGB_STATE[1]=128;
      RGB_STATE[2]=255;
      setRgbColor(RGB_STATE);
      strcpy(ttl, "Blue View");
      break;
    case 2:
      RGB_STATE[0]=255;
      RGB_STATE[1]=0;
      RGB_STATE[2]=0;
      setRgbColor(RGB_STATE);
      strcpy(ttl, "Red View");
      break;
    case 3:
      RGB_STATE[0]=255;
      RGB_STATE[1]=0;
      RGB_STATE[2]=255;
      setRgbColor(RGB_STATE);
      strcpy(ttl, "Purple View");
      break;
  }
  
  // load button states from current view 
  uint8_t shift = 6 * CURRENT_VIEW;  // 6*0=0; 6*1=6; 6*2=12...
  for(uint8_t i=0; i<6; i++) {
    scr.buttonState[i] = button_states[i+shift];
  }
  
  setTemporaryTitle(ttl);
  loadFxNames();
  buildScreen();
}

void showButtonStates() {

}

void loadFxNames() {
  uint8_t shift = 6*CURRENT_VIEW;  // 6*0=0; 6*1=6; 6*2=12...
  
  strcpy_P(scr.fxLabel1, (char*)pgm_read_word(&(gtModulesShortName[button2deviceMap[0+shift]])));
  strcpy_P(scr.fxLabel2, (char*)pgm_read_word(&(gtModulesShortName[button2deviceMap[1+shift]])));
  strcpy_P(scr.fxLabel3, (char*)pgm_read_word(&(gtModulesShortName[button2deviceMap[2+shift]])));
  strcpy_P(scr.fxLabel4, (char*)pgm_read_word(&(gtModulesShortName[button2deviceMap[3+shift]])));
  strcpy_P(scr.fxLabel5, (char*)pgm_read_word(&(gtModulesShortName[button2deviceMap[4+shift]])));
  strcpy_P(scr.fxLabel6, (char*)pgm_read_word(&(gtModulesShortName[button2deviceMap[5+shift]])));
  
  strcpy_P(scr.fxDescr1, (char*)pgm_read_word(&(gtModulesLongName[button2deviceMap[0+shift]])));
  strcpy_P(scr.fxDescr2, (char*)pgm_read_word(&(gtModulesLongName[button2deviceMap[1+shift]])));
  strcpy_P(scr.fxDescr3, (char*)pgm_read_word(&(gtModulesLongName[button2deviceMap[2+shift]])));
  strcpy_P(scr.fxDescr4, (char*)pgm_read_word(&(gtModulesLongName[button2deviceMap[3+shift]])));
  strcpy_P(scr.fxDescr5, (char*)pgm_read_word(&(gtModulesLongName[button2deviceMap[4+shift]])));
  strcpy_P(scr.fxDescr6, (char*)pgm_read_word(&(gtModulesLongName[button2deviceMap[5+shift]])));
  
}

/** ----------------------------------------------------------
 *          UTILS
 * ----------------------------------------------------------
 */
 
void patchUp(){
  if(PATCH_STATE < 125) { // 125 is the last patch, 126 and 127 are not used.
    PATCH_STATE++;
  } else {
    PATCH_STATE=1;  // loops to the first patch
  }
  sprintf (scr.title, "Patch %d", PATCH_STATE);
  load_default_states();
  loadView();
  MIDI.sendProgramChange (PATCH_STATE-1, channel); // counts from 0 that stands for the patch 1
  delay(5);
}

void patchDown() {
  if(PATCH_STATE > 1) {
    PATCH_STATE--;
  } else {
    PATCH_STATE=125;  // loops to the last patch
  }
  sprintf (scr.title, "Patch %d", PATCH_STATE);
  load_default_states();
  loadView();
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
  title_timeout_enabled=true;
  title_start_time = millis();
  strcpy(scr.title,tmpTitle);
  buildScreen();
}

/** ----------------------------------------------------------
 *          END UTILS
 * ----------------------------------------------------------
 */


/**----------------------------------------------------------
 *          MIDI READ
 ----------------------------------------------------------*/
/*void processPCmsg(byte channel, byte number, byte value) {
  //PATCH_STATE=number+1;
  char buf[8];
  sprintf (buf, "PC %d", number+1);
  strcpy(scr.title,buf);
  buildScreen();
}
void processCCmsg(byte channel, byte number, byte value) {
  char buf[8];
  sprintf (buf, "CC %d %d", number, value);
  strcpy(scr.title,buf);
  buildScreen();
}*/
/**----------------------------------------------------------
 *          END MIDI READ 
 * ----------------------------------------------------------*/
