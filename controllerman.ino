/**
 * ControllerMan
 * www.controllerman.com
 * 
 * Program tier is devided into 3 sections
 * ACTION > BEHAVIOR > SUBMISSION (device mapping)
 * 
 * ACTION takes the buttons input and generate the proper calls.
 * BEHAVIOR defines the relation between the action and the execution.
 * SUBMISSION contains all the commands device-specific to realize the control.
 * 
 */
#include <Bounce2.h>
#include "View.h"
#include "View_Green.h"
#include "View_Orange.h"
#include "View_Cyan.h"

#include "LibMIDI/MIDI.h"

const bool DEBUG = true;

const uint8_t ITEMS = 6;

// pinout configuration
const uint8_t PIN_BUTTON [ITEMS] = {18, 19, 20, 21, 11, 12}; // from 1st to 6th
const uint8_t PIN_STATE_LED [ITEMS] = {2, 3, 4, 5, 6, 7};  // from 1st to 6th
const uint8_t PIN_RGB_RED = 8;
const uint8_t PIN_RGB_GREEN = 9;
const uint8_t PIN_RGB_BLUE = 10;
uint8_t rgbState[3] = {0,255,0};

// Define possible button combinations
// First {2,4} means that button 1 can be combined as 1+2 and 1+4
const uint8_t DOUBLE_BUTT_COMBINATION[6][3] = {
          {2,4},    // butt 1
          {1,3,5},  // butt 2
          {2,6},    // butt 3
          {1,5},    // butt 4
          {4,2,6},  // butt 5
          {3,5}};   // butt 6


// Instantiate the Bounce objects
Bounce * DEBOUNCER = new Bounce[ITEMS];   // 6 debouncing objects

// Instantiate the views
View* VIEW;
uint8_t VIEW_INDEX=0;

void setup() {
  // Initialize the button pins
  for (int i = 0; i < ITEMS; i++) {
    pinMode(PIN_BUTTON[i] ,INPUT_PULLUP);
  }

  // Initialize led pins
  for (int i = 0; i < ITEMS; i++) {
     pinMode(PIN_STATE_LED[i], OUTPUT);
  }

  // Initialize RGB pins
  pinMode(PIN_RGB_RED, OUTPUT);
  pinMode(PIN_RGB_GREEN, OUTPUT);
  pinMode(PIN_RGB_BLUE, OUTPUT);

  // Initialize the bounce objects
  int bounceInterval = 5; // Millisec.
  for (int i = 0; i < ITEMS; i++) {
    DEBOUNCER[i].attach(PIN_BUTTON[i]);
    DEBOUNCER[i].interval(bounceInterval);
  }
  
  // Init the View
  VIEW = new View_Green();
  
  ledTest();
  
  VIEW->modifyColor(rgbState);
  setRgbColor(rgbState);
  
} // end Setup

void loop() {
  // Check all buttons
  for(uint8_t i=0; i<ITEMS; i++) {
    DEBOUNCER[i].update();
    if(DEBOUNCER[i].read()==LOW) { // button is pressed
      
      busyWarning(true);  // Warns the user that system is busy putting the rgb to red

      // Stops checking all the hardware focusing only on one control
      manageAction(i);
      
      busyWarning(false);
    }
    
  } // end for all buttons cycle
} // end loop()

/**
 * Manage the user action
 */
void manageAction(uint8_t i) {
  unsigned long press_time = millis();
  
  while(DEBOUNCER[i].read()==LOW) { // while button is released
    
    if((millis() - press_time) < 1000) {  // not a long press (... user haves one second to press a second button)
      
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
  VIEW->singlePress(button);
}
bool singlePress_hold(uint8_t button) {
  bool break_flag = VIEW->singlePress_hold(button);
  return break_flag;
}
void doublePress(uint8_t button1, uint8_t button2) {
  // sort the button numbers for convenience
  if(button1 > button2) { 
    uint8_t temp = button1;
    button1 = button2;
    button2 = temp;
  }
  
  if(button1==3 && button2==6) {  // combination reserved to scroll the view forward
    scroll_prev_view();
  } else if(button1==1 && button2==4) {  // combination reserved to scroll the view forward
    scroll_next_view();
  } else if(button1==2 && button2==5) {
    reset_view();
  } else {
    VIEW->doublePress(button1, button2);
  }
}
bool doublePress_hold(uint8_t button1, uint8_t button2) {
  // sort the button numbers for convenience
  if(button1 > button2) { 
    uint8_t temp = button1;
    button1 = button2;
    button2 = temp;
  }
  bool break_flag = VIEW->doublePress_hold(button1, button2);
  return break_flag;
}



void scroll_prev_view() {
  if(VIEW_INDEX==0) { // Green view
    VIEW_INDEX=1;
    VIEW=new View_Orange();
    VIEW->modifyColor(rgbState);
    setRgbColor(rgbState);
  } else if(VIEW_INDEX==1) {  // Orange view
    VIEW_INDEX=2;
    VIEW=new View_Cyan();
    VIEW->modifyColor(rgbState);
  setRgbColor(rgbState);
  } else if(VIEW_INDEX==2) {  // Cyan view
    VIEW_INDEX=0;
    VIEW=new View_Green();
    VIEW->modifyColor(rgbState);
    setRgbColor(rgbState);
  }
  ledSwipeRight();
}
void scroll_next_view() {
  if(VIEW_INDEX==2) { // Cyan view
    VIEW_INDEX=1;
    VIEW=new View_Orange();
    VIEW->modifyColor(rgbState);
    setRgbColor(rgbState);
  } else if(VIEW_INDEX==1) {  // Orange view
    VIEW_INDEX=0;
    VIEW=new View_Green();
    VIEW->modifyColor(rgbState);
    setRgbColor(rgbState);
  } else if(VIEW_INDEX==0) {  // Green view
    VIEW_INDEX=2;
    VIEW=new View_Cyan();
    VIEW->modifyColor(rgbState);
    setRgbColor(rgbState);
  }
  ledSwipeLeft();
}
void reset_view() {
  if(VIEW_INDEX != 0) {
    VIEW_INDEX=0;
    VIEW=new View_Green();
    VIEW->modifyColor(rgbState);
    setRgbColor(rgbState);
  }
  ledResetView();
}


/**
 * UTILS  -----------------------------------------------------------------
 */

/** 
 *  Enable or disable the busy signal (red rgb led)
 */
uint8_t busyColor[3] = {0,0,0};
void busyWarning(bool enabled) {
  if(enabled) {
    setRgbColor(busyColor);
  } else {
    setRgbColor(rgbState);
  }
}

void setRgbColor(uint8_t c[]) {
  analogWrite(PIN_RGB_RED, c[0]);
  analogWrite(PIN_RGB_GREEN, c[1]);
  analogWrite(PIN_RGB_BLUE, c[2]);
}

/**
 * ledTest
 * Create an animation to check the leds
 */
void ledTest() {
  int ledWait=50;
  for(int i=0; i<ITEMS; i++) {
    digitalWrite(PIN_STATE_LED[i], HIGH);
    delay(ledWait);
  }
  for(int i=0; i<ITEMS; i++) {
    digitalWrite(PIN_STATE_LED[i], LOW);
    delay(ledWait);
  }

  // RGB animation
  int rgbWait=500;
  for(int i=0; i<256; i++){
    analogWrite(PIN_RGB_RED, i);
    delayMicroseconds(rgbWait);
  }
  for(int i=0; i<256; i++){
    analogWrite(PIN_RGB_GREEN, i);
    delayMicroseconds(rgbWait);
  }
  for(int i=0; i<256; i++){
    analogWrite(PIN_RGB_BLUE, i);
    delayMicroseconds(rgbWait);
  }
  for(int i=255; i>=0; i--){
    analogWrite(PIN_RGB_RED, i);
    delayMicroseconds(rgbWait);
  }
  for(int i=255; i>=0; i--){
    analogWrite(PIN_RGB_GREEN, i);
    delayMicroseconds(rgbWait);
  }
  for(int i=255; i>=0; i--){
    analogWrite(PIN_RGB_BLUE, i);
    delayMicroseconds(rgbWait);
  }
} // end led test

void ledSwipeLeft() {
  int ledWait=20;
  for(int i=0; i<ITEMS; i++) {
    digitalWrite(PIN_STATE_LED[i], HIGH);
    delay(ledWait);
  }
  for(int i=0; i<ITEMS; i++) {
    digitalWrite(PIN_STATE_LED[i], LOW);
    delay(ledWait);
  }
}
void ledSwipeRight() {
  int ledWait=20;
  for(int i=ITEMS-1; i>=0; i--) {
    digitalWrite(PIN_STATE_LED[i], HIGH);
    delay(ledWait);
  }
  for(int i=ITEMS-1; i>=0; i--) {
    digitalWrite(PIN_STATE_LED[i], LOW);
    delay(ledWait);
  }
}
void ledResetView() {
  digitalWrite(PIN_STATE_LED[0], HIGH);
  digitalWrite(PIN_STATE_LED[2], HIGH);
  digitalWrite(PIN_STATE_LED[3], HIGH);
  digitalWrite(PIN_STATE_LED[5], HIGH);
  digitalWrite(PIN_STATE_LED[1], LOW);
  digitalWrite(PIN_STATE_LED[4], LOW);
  delay(100);
  digitalWrite(PIN_STATE_LED[1], HIGH);
  digitalWrite(PIN_STATE_LED[4], HIGH);
  digitalWrite(PIN_STATE_LED[0], LOW);
  digitalWrite(PIN_STATE_LED[2], LOW);
  digitalWrite(PIN_STATE_LED[3], LOW);
  digitalWrite(PIN_STATE_LED[5], LOW);
  delay(100);
  digitalWrite(PIN_STATE_LED[1], LOW);
  digitalWrite(PIN_STATE_LED[4], LOW);
  
}
