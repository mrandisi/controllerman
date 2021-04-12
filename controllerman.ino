#include <Bounce2.h>

const bool DEBUG = true;

const uint8_t ITEMS = 6;

// pinout configuration
const uint8_t PIN_BUTTON [ITEMS] = {18, 19, 20, 21, 11, 12}; // from 1st to 6th
const uint8_t PIN_STATE_LED [ITEMS] = {2, 3, 4, 5, 6, 7};  // from 1st to 6th
const uint8_t PIN_RGB_RED = 8;
const uint8_t PIN_RGB_GREEN = 9;
const uint8_t PIN_RGB_BLUE = 10;

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

struct Color {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

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
  
  if(DEBUG) {
    Serial.println("System ready!");
  }
  
  ledTest();

  Color greenColor = {0,255,0};
  setRgbColor(greenColor);
  
} // end Setup

void loop() {
  // Check all buttons
  for(uint8_t i=0; i<ITEMS; i++) {
    DEBOUNCER[i].update();
    if(DEBOUNCER[i].read()==LOW) { // button is pressed
      
      busyWarning(true);  // Warns the user that system is busy putting the rgb to red

      // Stops checking all the hardware focusing only on one control
      manageButton(i);
      
      busyWarning(false);
    }
    
  } // end for all buttons cycle
} // end loop()

/**
 * Manage the user action
 */
void manageButton(uint8_t i) {
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
              bool break_flag = doublePress_hold(i+1,nearButton);

              if(break_flag) {  // wait for buttons release
                while(DEBOUNCER[i].read()==LOW || DEBOUNCER[nearButton-1].read()==LOW) {
                  DEBOUNCER[i].update();
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

  
}

void singlePress(uint8_t button) {
  if(DEBUG) {
    Serial.print("Single press: ");
    Serial.println(button, DEC);
  }
}
bool singlePress_hold(uint8_t button) {
  bool break_flag = true;
  if(DEBUG) {
    Serial.print("Long press: ");
    Serial.println(button, DEC);
  }
  return break_flag;
}
void doublePress(uint8_t button1, uint8_t button2) {
  if(DEBUG) {
    Serial.print("Double press: ");
    Serial.print(button1, DEC);
    Serial.print("+");
    Serial.println(button2, DEC);
  }
}
bool doublePress_hold(uint8_t button1, uint8_t button2) {
  bool break_flag = true;
  if(DEBUG) {
    Serial.print("Long double press: ");
    Serial.print(button1, DEC);
    Serial.print("+");
    Serial.println(button2, DEC);
  }
  return break_flag;
}





/**
 * UTILS  -----------------------------------------------------------------
 */

/** 
 *  Enable or disable the busy signal (red rgb led)
 */
Color greenColor = {0,255,0};
Color redColor = {255, 0, 0};
void busyWarning(bool enabled) {
  if(enabled) {
    setRgbColor(redColor);
  } else {
    setRgbColor(greenColor);
  }
}
/**
 * Shortcut to control the rgb color
 */
void setRgbColor(Color c) {
  analogWrite(PIN_RGB_RED, c.red);
  analogWrite(PIN_RGB_GREEN, c.green);
  analogWrite(PIN_RGB_BLUE, c.blue);
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
