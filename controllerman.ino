#include <Bounce2.h>

#define ITEMS 6

const bool debug = true;

// pinout configuration
const uint8_t PIN_BUTTON [ITEMS] = {18, 19, 20, 21, 11, 12}; // from 1st to 6th
const uint8_t PIN_STATE_LED [ITEMS] = {2, 3, 4, 5, 6, 7};  // from 1st to 6th
const uint8_t PIN_RGB_RED = 8;
const uint8_t PIN_RGB_GREEN = 9;
const uint8_t PIN_RGB_BLUE = 10;

// Define possible button combinations
// 0 means button 1, can be combined as 1+2 and 1+4
const uint8_t combination1[] = {2,4};
const uint8_t combination2[] = {1,3,5};
const uint8_t combination3[] = {2,6};
const uint8_t combination4[] = {1,5};
const uint8_t combination5[] = {4,2,6};
const uint8_t combination6[] = {3,5};
const uint8_t DOUBLE_BUTT_COMBINATION[][1] = {*combination1, *combination2, *combination3, *combination4, *combination5, *combination6};

// Instantiate the Bounce objects
Bounce * debouncer = new Bounce[ITEMS];   // 6 debouncing objects

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
    debouncer[i].attach(PIN_BUTTON[i]);
    debouncer[1].interval(bounceInterval);
  }
  
  if(debug) Serial.println("System ready!");
  
  ledTest();
} // end Setup

void loop() {
  
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
  int rgbWait=1;
  for(int i=0; i<256; i++){
    analogWrite(PIN_RGB_RED, i);
    delay(rgbWait);
  }
  for(int i=0; i<256; i++){
    analogWrite(PIN_RGB_GREEN, i);
    delay(rgbWait);
  }
  for(int i=0; i<256; i++){
    analogWrite(PIN_RGB_BLUE, i);
    delay(rgbWait);
  }
  for(int i=255; i>=0; i--){
    analogWrite(PIN_RGB_RED, i);
    delay(rgbWait);
  }
  for(int i=255; i>=0; i--){
    analogWrite(PIN_RGB_GREEN, i);
    delay(rgbWait);
  }
  for(int i=255; i>=0; i--){
    analogWrite(PIN_RGB_BLUE, i);
    delay(rgbWait);
  }
}
