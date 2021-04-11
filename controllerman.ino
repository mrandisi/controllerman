#include <Bounce2.h>
#include "BehaviorView_main.h"

#define ITEMS 6

const bool debug = true;

// pinout configuration
const uint8_t PIN_BUTTON [ITEMS] = {18, 19, 20, 21, 11, 12}; // from 1st to 6th
const uint8_t PIN_STATE_LED [ITEMS] = {2, 3, 4, 5, 6, 7};  // from 1st to 6th
const uint8_t PIN_RGB_RED = 8;
const uint8_t PIN_RGB_GREEN = 9;
const uint8_t PIN_RGB_BLUE = 10;

// Define possible button combinations
// First {2,4,0} means that button 1 can be combined as 1+2 and 1+4, 0 is the array terminator.
// [6][4] means 6 buttons and Maximum 3 combinations + the terminator.
const uint8_t DOUBLE_BUTT_COMBINATION[6][4] = {
          {2,4,0},    // butt 1
          {1,3,5,0},  // butt 2
          {2,6,0},    // butt 3
          {1,5,0},    // butt 4
          {4,2,6,0},  // butt 5
          {3,5,0}};   // butt 6


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
  
  if(debug) {
    Serial.println("System ready!");
  }
  
  ledTest();

  Color greenColor = buildRgbColor(0,255,0);
  setRgbColor(greenColor);
  
} // end Setup

void loop() {
  checkAllButtons();
}

void checkAllButtons() {
  for(int i=0; i<ITEMS; i++) {
    DEBOUNCER[i].update();
    
    if(DEBOUNCER[i].read()==LOW) { // button is pressed
      /**
       * SECOND LEVEL ALGORITHM
       * stops checking all the buttons
       * focusing only on himself and the neighbors
       */
      if(debug) {
        Serial.print("Button is down: ");
        Serial.println(i+1, DEC);
      }
      
      unsigned long press_time = millis();
      busyWarning(true);  // Warns the user that system is busy putting the rgb to red
      
      while(DEBOUNCER[i].read()==LOW) { // while button is released
        
        if((millis() - press_time) < 1000) {  // not a long press (... user haves one second to press a second button)
          // check nearby buttons
          
          uint8_t j=0;
          while(DOUBLE_BUTT_COMBINATION[i][j] > 0) {  // 0 is the terminator

            uint8_t nearButton = DOUBLE_BUTT_COMBINATION[i][j];
            
            DEBOUNCER[nearButton-1].update();

            if(DEBOUNCER[nearButton-1].read()==LOW) { // second button is pressed
              unsigned long double_press_time = millis();
              if(debug) {
                Serial.print("Combined button is down: ");
                Serial.print(i+1, DEC);
                Serial.print("+");
                Serial.println(nearButton, DEC);
              }
              
              // wait for release of both buttons
              while(DEBOUNCER[i].read()==LOW || DEBOUNCER[nearButton-1].read()==LOW) {

                // Double long press
                // UNUSED
                /*if((millis() - double_press_time) > 1000) {
                  // Run double button long press action
                  if(debug) {
                    Serial.print("Combined long press is down: ");
                    Serial.print(i+1, DEC);
                    Serial.print("+");
                    Serial.println(nearButton, DEC);
                  }
                }*/
                
                DEBOUNCER[i].update();
                DEBOUNCER[nearButton-1].update();
              }

              //if((millis() - double_press_time) <= 1000) {  // only for double long press
                // RUN DOUBLE BUTTON i,j ACTION FOR CURRENT VIEW
                if(debug) {
                  Serial.print("Combined b. click: ");
                  Serial.print(i+1, DEC);
                  Serial.print("+");
                  Serial.println(nearButton, DEC);
                 }
                
              //}
              
              return;
            }
            
            j++;
          }
          
          

        } else {  // REPETITIVE LONG PRESS DETECTED
          /*
           * While button is pressed
           * system continues to call this field.
           * 
           */
          if(debug) {
            Serial.print("Long press detected: ");
            Serial.println(i+1, DEC);
          }
        }
        
        DEBOUNCER[i].update();  // update the state of the pressed button
      }
      // primary button released (without double button combination)
      if((millis() - press_time) < 1000) {
        //  RUN SIMPLE ACTION
        if(debug) {
          Serial.print("Single click detected on Butt. ");
          Serial.println(i+1, DEC);
        }
        //return;
      }
    } // end if button pressed

    busyWarning(false);
  } // end for all buttons cycle
  return;
}

/**
 * Enable or disable the busy signal (red rgb led)
 */
Color TEMP_COLOR;
Color redColor = buildRgbColor(255, 0, 0);
void busyWarning(boolean enabled) {
  if(enabled) {
    TEMP_COLOR = buildRgbColor(analogRead(PIN_RGB_RED), analogRead(PIN_RGB_GREEN), analogRead(PIN_RGB_BLUE));
    
    setRgbColor(redColor);
  } else {
    setRgbColor(TEMP_COLOR);
  }
}
/**
 * Build a RGB color struct
 */
Color buildRgbColor(uint8_t red, uint8_t green, uint8_t blue) {
  Color c;
  c.red = red;
  c.green = green;
  c.blue = blue;

  return c;
}
void setRgbColor(Color c) {
  analogWrite(PIN_RGB_RED, c.red);
  analogWrite(PIN_RGB_GREEN, c.green);
  analogWrite(PIN_RGB_BLUE, c.blue);
}









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