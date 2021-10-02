/**
 * ControllerMan
 * 
 */
#include <Bounce2.h>      // buttons anti bounce
#include <avr/pgmspace.h> // Provides PROGMEM to read directly from the ROM
#include <MIDI.h>
#include "DeviceSettings.h"
#include "Display.h"


//MIDI_CREATE_DEFAULT_INSTANCE();
MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI); //Serial1 if usb

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


bool TITLE_TIMEOUT_ENABLED=false;
unsigned long TITLE_START_TIME = 0;
bool EDIT_MODE=false;
unsigned long LAZY_TIME=millis();
uint8_t VERY_SLOW_COUNTER=0;

bool UPDATE_SCREEN=false;
bool IMPORT_PATCH=false;

// Instantiate the Bounce objects
Bounce * DEBOUNCER = new Bounce[BUTTQTY];   // 6 debouncing objects

void setup() {

  if(isFirstRun()) {
    analogWrite(PIN_RGB_GREEN, 255);
    delay(3000);
    reset_device();
  }
  
  MIDI.begin();
  //MIDI.begin(MIDI_CHANNEL_OMNI); // Initialize the Midi Library.
  //MIDI.setHandleControlChange(processCCmsg); // This command tells the MIDI Library
  //MIDI.setHandleProgramChange(processPCmsg); // This command tells the MIDI Library
  
  // Initialize the button pins
  for (int i = 0; i < BUTTQTY; i++) {
    pinMode(PIN_BUTTON[i] ,INPUT_PULLUP);
  }
  //pinMode( 0, INPUT_PULLUP );

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

  CHANNEL=read_outChannel();

  // boot animation
  //splash();
  ledBlink(/*ms_on*/3,/*ms_off*/0,/*repeat*/1, LAYOUT_COLOR[0]);

  PATCH_STATE = read_default_patch();
  sprintf (scr.title, "P%03d", PATCH_STATE);
  load_default_states(PATCH_STATE);
  
  UPDATE_SCREEN=true;
  
  // Synchronize all data at the beginning
  syncDevice();

  init_display();
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
    
    VERY_SLOW_COUNTER++;
    if(VERY_SLOW_COUNTER > 100) { // 10*100=one second
      VERY_SLOW_COUNTER=0;

    if(UPDATE_SCREEN) {
      loadLayout();
      UPDATE_SCREEN=false;
    } else if(IMPORT_PATCH) {
      sprintf (scr.title, "P%03d", PATCH_STATE);
      load_default_states(PATCH_STATE);
      loadLayout();
      IMPORT_PATCH=false;
    }

      // opens the menu
      if(EDIT_MODE) {
        while(settings_menu());
        EDIT_MODE=false;
        drawLayout(scr);
      }
  
      // title timeout restore
      if(TITLE_TIMEOUT_ENABLED) {
        unsigned long nowTime = millis();
        if(nowTime-TITLE_START_TIME > 1000) {
          scr.ccNum=0;
          sprintf(scr.title, "P%03d", PATCH_STATE);
          drawLayout(scr);
          TITLE_TIMEOUT_ENABLED=false;
        }
      }
      
    }
  }
  
} // end loop()


/**----------------------------------------------------------
 *          MIDI READ
 ----------------------------------------------------------*/
/*void processPCmsg(byte channel, byte number, byte value) {
  ledBlink(3,0,1, LAYOUT_COLOR[2]);
  if(PATCH_STATE != number+1) {
    if(number >=0 && number <125) {
      PATCH_STATE=number+1;
      LAZY_TIME=millis();
      IMPORT_PATCH=true;
    } else {
      ledBlink(3,0,1, LAYOUT_COLOR[0]);
    }

    //ledBlink(/3,0,1, LAYOUT_COLOR[0]);
  }
  
  //PATCH_STATE=number+1;
  //char buf[8];
  //sprintf (buf, "PC %d", number);
  //setTemporaryTitle(buf);
}*/

//void processCCmsg(byte channel, byte number, byte value) {

//  ledBlink(/*ms_on*/3,/*ms_off*/0,/*repeat*/1,LAYOUT_COLOR[0]);
  
/*  char buf[8];
  sprintf (buf, "CC %d", number);
  setTemporaryTitle(buf);
}*/
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
  const long int startTime = millis();
  long int nowTime=startTime;
  while(nowTime-startTime < delayTime) {

    //updateRealtimeProcesses();
    
    nowTime = millis();
  }
}

/*void updateRealtimeProcesses() {
  MIDI.read(); // Continuously check if Midi data has been received.
}*/

void manageAction(uint8_t i) {
  unsigned long press_time = millis();

  uint8_t doubleClickEnabled = getFxDoubleClickLink( i + (6*(CURRENT_LAYOUT)) );
  uint8_t longPressEnabled = getFxLongPressLink( i + (6*(CURRENT_LAYOUT)) );
  
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
//                  updateRealtimeProcesses();
                }
              } // end if wait
              
            }
            DEBOUNCER[i].update();
            DEBOUNCER[nearButton-1].update();
//            updateRealtimeProcesses();
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

      if(longPressEnabled) {
        // System continues to call this field
        // while button is released or action returns break=true;
        bool break_flag = singlePress_hold(i+1);

        if(break_flag) {  // wait for button release
          while(DEBOUNCER[i].read()==LOW) {
            DEBOUNCER[i].update();
//            updateRealtimeProcesses();
          }
        }
      }
    }
    
    DEBOUNCER[i].update();  // update the state of the pressed button
//    updateRealtimeProcesses();
  }
  // primary button released (without double button combination)
  if((millis() - press_time) < 1000) {
    bool isDoubleClick=false;

    // Detect doubleCLick;
    if(doubleClickEnabled) { // double click enabled for that button
      press_time = millis();
      while((millis() - press_time) < 250) {  // 250ms between the release and a press
        DEBOUNCER[i].update();

        if(DEBOUNCER[i].read()==LOW) {  // double click detected
          isDoubleClick=true;

          doubleClick(i+1);

          // wait for release...
          while((DEBOUNCER[i].read()==LOW)) {
            DEBOUNCER[i].update();
//            updateRealtimeProcesses();
          }
        }
//        updateRealtimeProcesses();
      }
    }
    if(!isDoubleClick) {
      singlePress(i+1);
    }
    
  }
  
} // end manage action

void doubleClick(uint8_t button) {
  uint8_t virtual_button_index = button + (6*(CURRENT_LAYOUT)) - 1; // from 0 to 23
  uint8_t vbutton_link = getFxDoubleClickLink(virtual_button_index);

  toggleButton(vbutton_link-1);
}

void singlePress(uint8_t button) {
  uint8_t virtual_button_index = (6 * CURRENT_LAYOUT) + (button-1);

  toggleButton(virtual_button_index);
}

bool singlePress_hold(uint8_t button) {
  
  uint8_t virtual_button_index = button + (6*(CURRENT_LAYOUT)) - 1; // from 0 to 23
  uint8_t vbutton_link = getFxLongPressLink(virtual_button_index);

  toggleButton(vbutton_link-1);
  
  return true; //break_flag;
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
    //patchUp();
    PATCH_STATE++;
    setPatch();
  } else if(button1==2 && button2==3) {
    //patchDown();
    PATCH_STATE--;
    setPatch();
  } else if(button1==4 && button2==5) {  // jump patch up/down
    //jumpPatchUp();
    PATCH_STATE+=10;
    setPatch();
  } else if(button1==1 && button2==2) {
    //jumpPatchDown();
    PATCH_STATE=(PATCH_STATE==1)?125:((PATCH_STATE<12)?1:PATCH_STATE-=10);
    setPatch();
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
    //patchUp();
    PATCH_STATE++;
    setPatch();
    delay(50);
    return false;
  } else if(button1==2 && button2==3) {
    //patchDown();
    PATCH_STATE--;
    setPatch();
    delay(50);
    return false;
  } else if(button1==4 && button2==5) {  // reserved to jump patch up/down
    //jumpPatchUp();
    PATCH_STATE+=10;
    setPatch();
    delay(50);
    return false;
  } else if(button1==1 && button2==2) {
    //jumpPatchDown();
    PATCH_STATE=(PATCH_STATE==1)?125:((PATCH_STATE<12)?1:PATCH_STATE-=10);
    
    setPatch();
    delay(50);
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

void toggleButton(uint8_t virtual_button) {
  char ln[11];
  getFxLongName(virtual_button, ln);
  uint8_t ccnum = getFxCCnum(virtual_button);
  char buf[16];
  
  if(BUTTON_STATES[virtual_button]==false) {  // is inactive
    BUTTON_STATES[virtual_button]=true;
    MIDI.sendControlChange(ccnum, 127, CHANNEL); // activate 
    sprintf(buf, "%s ON", ln);
  } else {                    // is active
    BUTTON_STATES[virtual_button]=false;
    MIDI.sendControlChange(ccnum, 0, CHANNEL);
    sprintf(buf, "%s OFF", ln);
  } // end if false
  scr.ccNum=ccnum;
  setTemporaryTitle(buf);
  
  //loadLayout();
  UPDATE_SCREEN=true;
}

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
  setRgbColor(LAYOUT_COLOR[CURRENT_LAYOUT]);
  
  strcpy_P(LAYOUT_TITLE, (char*)pgm_read_word(&( layoutName[CURRENT_LAYOUT] )));
  strcat(LAYOUT_TITLE, " layout");
  
  // load button states from current layout 
  uint8_t shift = 6 * CURRENT_LAYOUT;  // 6*0=0; 6*1=6; 6*2=12...
  for(uint8_t i=0; i<6; i++) {
    scr.buttonState[i] = BUTTON_STATES[i+shift];
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

void setPatch(){
  if(PATCH_STATE > 125) { // 125 is the last patch, 126 and 127 are not used.
    PATCH_STATE=1;
  } else if(PATCH_STATE < 1) {
    PATCH_STATE=125;  // loops to the first patch
  }
  sprintf (scr.title, "P%03d", PATCH_STATE);
  load_default_states(PATCH_STATE);
  loadLayout();
  //UPDATE_SCREEN=true;
  syncDevice();
}

void syncDevice() {
  // send the patch number
  MIDI.sendProgramChange (PATCH_STATE-1, CHANNEL); // counts from 0 that stands for the patch 1
  delay(8);
  for(uint8_t i=0; i<24; i++) {
      if(BUTTON_STATES[i]==false) {
        MIDI.sendControlChange(getFxCCnum(i), 0, CHANNEL);
      } else {
        MIDI.sendControlChange(getFxCCnum(i), 127, CHANNEL);
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
  //drawLayout(scr);
  UPDATE_SCREEN=true;
}

void ledBlink(uint8_t ms_on, uint8_t ms_off, uint8_t repeat, uint8_t * color) {
  for(uint8_t i=0; i<repeat; i++) {
    analogWrite(PIN_RGB_RED, color[0]);
    analogWrite(PIN_RGB_GREEN, color[1]);
    analogWrite(PIN_RGB_BLUE, color[2]);
    delay(ms_on);
    setRgbColor(LAYOUT_COLOR[CURRENT_LAYOUT]);
    delay(ms_off);
  }
} // end led Blink*/

/** ----------------------------------------------------------
 *          END UTILS
 * ----------------------------------------------------------*/



/** ----------------------------------------------------------
 *          SETTINGS MENU
 * ----------------------------------------------------------*/

uint8_t checkButtons() {
  uint8_t btChoose=0;

  while(btChoose == 0) {
      
    for(uint8_t i=0; i<6; i++) {
      DEBOUNCER[i].update();
      if(DEBOUNCER[i].read()==LOW) { // button is pressed
        
        // hook waiting for release 180ms, then jumps to the next
        uint8_t count = 0;
        while(DEBOUNCER[i].read()==LOW && count < 180) {
          DEBOUNCER[i].update();
          delay(1);
          if(i==1 || i==4) {  // only up/dpwn
            count++;
          }
        }

        btChoose = i+1; // button number
        return btChoose;
      }
    } // end for all buttons cycle
  } // end while
  return 0;
}

uint8_t settings_menu() {
  EDIT_MODE=true;
  uint8_t choice=0;
  
  choice = listChooser(/*property*/0, /*menu_id*/1, /*defaultEntry*/1);

  if(choice==0) {
    return 0;
  } else if(choice==1) { // save button states
    setTemporaryTitle("Saved patch states");
    write_default_states(PATCH_STATE);
    return 0;

  } else if(choice==2) { // swap
    
    uint8_t fromVButton = listChooser(/*property*/1, /*menu_id*/3, /*defaultEntry*/1);
    if(!fromVButton) return 1;

    uint8_t toVButton = listChooser(/*property*/2, /*menu_id*/3, /*defaultEntry*/1);
    if(!toVButton) return 1;
    
    swapButtons(fromVButton, toVButton);
    swapStates(fromVButton, toVButton);
    loadLayout();
    
    setTemporaryTitle("Swap done!");
    return 0;
    
  } else if(choice==3) { // Edit button
    
    uint8_t virtual_button = listChooser(/*property*/3, /*menu_id*/3, /*defaultEntry*/1);
    
    if(!virtual_button) return 1;

    if(editButton(virtual_button)) {
      setTemporaryTitle("Saving patch state");
      loadLayout();
      return 0;
    }
    return 1;

  } else if(choice==4) { // Set boot patch

      uint8_t virtual_button = listChooser(/*property*/4, /*menu_id*/4, /*defaultEntry*/read_default_patch());
      if(virtual_button>0) {
        write_default_patch(virtual_button);
        char str[20];
        sprintf(str, "Will boot on P. %d", virtual_button);
        setTemporaryTitle(str);
        return 0;
      }
      return 1;

  } else if(choice==5) { // Set out channel

      uint8_t virtual_button = listChooser(/*property*/5, /*menu_id*/5, /*defaultEntry*/read_outChannel());
      if(virtual_button>0) {
        write_outChannel(virtual_button);
        char str[20];
        sprintf(str, "Out channel: %d", virtual_button);
        setTemporaryTitle(str);
        return 0;
      }
      return 1;
      
  } else if(choice==6) { // Reset
    drawConfirm(6);
    delay(2000);
    int8_t butt_choice = checkButtons();  // waits for a choose
    if(butt_choice == 1) {
      //drawConfirm(7);
      reset_device();
    }
    return 1;
  }
  return 1;
}

void swapStates(uint8_t fromVButton, uint8_t toVButton) {  // 1 to 24
  fromVButton--;
  toVButton--;

  for(uint8_t i=1; i<=125; i++) {
    load_default_states(i);

    if(BUTTON_STATES[fromVButton] != BUTTON_STATES[toVButton]) {  // if different swap and save
      bool tmp = BUTTON_STATES[fromVButton];
      BUTTON_STATES[fromVButton] = BUTTON_STATES[toVButton];
      BUTTON_STATES[toVButton] = tmp;

      write_default_states(i);
    }
  }
  load_default_states(PATCH_STATE);
}

void swapButtons(uint8_t fromVButton, uint8_t toVButton) {  // 1 to 24
  fromVButton--;
  toVButton--;

  // put TO into tmp
  byte tmpCCnum = getFxCCnum(toVButton);
  char tmpShortName[3];
  getFxShortName(toVButton, tmpShortName);
  char tmpLongName[10];
  getFxLongName(toVButton, tmpLongName);
  
  // metto FROM in TO
  byte fromCCnum = getFxCCnum(fromVButton);
  char fromShortName[4];
  getFxShortName(fromVButton, fromShortName);
  char fromLongName[11];
  getFxLongName(fromVButton, fromLongName);
  
  setFxCCnum(toVButton, fromCCnum);
  writeFxShortName(toVButton, fromShortName);
  setFxLongName(toVButton, fromLongName);

  // put tmp in FROM
  setFxCCnum(fromVButton, tmpCCnum);
  writeFxShortName(fromVButton, tmpShortName);
  setFxLongName(fromVButton, tmpLongName);
  
}

bool editButton(uint8_t virtual_button) { // 1 to 24
  char fx[17];
  getFx(virtual_button-1, fx);
  uint8_t selectedIndex=0;
  
  while(true) {
    drawButtonEdit(selectedIndex, fx);

    uint8_t butt_choice = checkButtons();  // waits for a choose

    if(butt_choice==5) {       // go up
        fx[selectedIndex]+=1;
    }else if(butt_choice==2) { // go down
        fx[selectedIndex]-=1;
    } else if(butt_choice==4) { // go left
      if(selectedIndex>0) {
        selectedIndex--;
      }
    } else if(butt_choice==6) { // go right
      if(selectedIndex<15) {
        selectedIndex++;
      }
    } else if(butt_choice==1) { // return and save changes
      writeFx(virtual_button-1, fx);
      return true;
    } else if(butt_choice==3) { // return and discard changes
      return false;
    }

  }
  
  return false;
}

uint8_t listChooser(uint8_t property, uint8_t menu_id, uint8_t defaultEntry) {

  uint8_t menuLen = getPMStrSize(menu_id);
  
  uint8_t m_selected_entry=defaultEntry;  // 1 to n
  uint8_t firstItemShift=0;
  uint8_t butt_choice = 0;

  uint8_t menu_entries=7;
  if(menuLen<7) {
    menu_entries = menuLen;
  }

  if(m_selected_entry > 7) {
    firstItemShift=m_selected_entry-4;
    if(firstItemShift+7 > menuLen) {
      firstItemShift=menuLen-7;
    }
  }
  
  while(butt_choice==0 || butt_choice==5 || butt_choice==2) {   // cancel, up, down

    drawListChooser(property, menu_id, m_selected_entry, firstItemShift);
    
    butt_choice = 0;
    butt_choice = checkButtons();  // waits for a choose
    
    if(butt_choice==2) {       // go up

      if(m_selected_entry < menuLen) {
        m_selected_entry++;
        if(m_selected_entry-firstItemShift > menu_entries) {
          firstItemShift=m_selected_entry-menu_entries;
        }
      } else {
        m_selected_entry=1;
        firstItemShift=0;
      }

    }else if(butt_choice==5) { // go down
      
      if(m_selected_entry > 1) {
        m_selected_entry--;
        
        if(m_selected_entry-firstItemShift < 1) {
          firstItemShift=m_selected_entry-1;
        }
      } else {
        m_selected_entry=menuLen;
        firstItemShift=menuLen-menu_entries;
      }

    } else if(butt_choice==1) {  // confirm
      return m_selected_entry;

    } else if(butt_choice==3) {   // cancel
      return 0;

    }

  } // end while
  
  return 0;
}

/** ----------------------------------------------------------
 *          END SETTINGS MENU
 * ----------------------------------------------------------*/
