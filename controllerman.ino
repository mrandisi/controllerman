#include <Bounce2.h>

#define ITEMS 6

const bool debug = true;

// pinout configuration
const uint8_t PIN_BUTTON [ITEMS] = {18, 19, 20, 21, 11, 12}; // from 1st to 6th
const uint8_t PIN_STATE_LED [ITEMS] = {2, 3, 4, 5, 6, 7};  // from 1st to 6th
const uint8_t PIN_RGB_RED = 8;
const uint8_t PIN_RGB_GREEN = 9;
const uint8_t PIN_RGB_BLUE = 10;

void setup() {

  // Initialize the button pins
  for (int i = 0; i < ITEMS; i++) {
    pinMode(PIN_BUTTON[i] ,INPUT_PULLUP);
  }

  // Initialize led pins
  for (int i = 0; i < ITEMS; i++) {
     pinMode(PIN_STATE_LED[i], OUTPUT);
  }

  if(debug) Serial.println("System ready!");
  
  
} // end Setup

void loop() {
  
}
