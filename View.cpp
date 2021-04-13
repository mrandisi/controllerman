/**
 * GreenView
 */

#include "Arduino.h"
#include "View.h"

View::View()
{
  
}
uint8_t View::getColor() {
  //return this->color;
  uint8_t a[3] = {this->red, this->green, this->blue};
  return a;
}
uint8_t View::getRed() {
  return this->red;
}
uint8_t View::getGreen() {
  return this->green;
}
uint8_t View::getBlue() {
  return this->blue;
}

void View::singlePress(uint8_t button) {
    if(DEBUG) {
    Serial.print("Single press: ");
    Serial.println(button, DEC);
  }
  switch (button) {
    case 1:
      butt1_singlePress();
      break;
    case 2:
      // statements
      break;
    case 3:
      // statements
      break;
    case 4:
      // statements
      break;
    case 5:
      // statements
      break;
    case 6:
      // statements
      break;
  
    default:
      // statements
      break;
  }
}
bool View::singlePress_hold(uint8_t button) {
  if(DEBUG) {
    Serial.print("Long press: ");
    Serial.println(button, DEC);
  }
  
  switch (button) {
    case 1:
      butt1_singlePress();
      break;
    case 2:
      // statements
      break;
    case 3:
      // statements
      break;
    case 4:
      // statements
      break;
    case 5:
      // statements
      break;
    case 6:
      // statements
      break;
  
    default:
      // statements
      break;
  }
  
  bool break_flag = true;
  return break_flag;
}
void View::doublePress(uint8_t button1, uint8_t button2) {
  if(DEBUG) {
    Serial.print("Double press: ");
    Serial.print(button1, DEC);
    Serial.print("+");
    Serial.println(button2, DEC);
  }
  uint8_t c = button1+button2;
  switch (c) {
    case 3:
      // statements
      break;
    case 5:
      // statements
      break;
    case 9:
      // statements
      break;
    case 11:
      // statements
      break;
  
    default:
      // statements
      break;
  }
}
bool View::doublePress_hold(uint8_t button1, uint8_t button2) {
  if(DEBUG) {
    Serial.print("Long double press: ");
    Serial.print(button1, DEC);
    Serial.print("+");
    Serial.println(button2, DEC);
  }
  uint8_t c = button1+button2;
  switch (c) {
    case 3: // button 1 + button 2
      // statements
      break;
    case 5: // 2+3
      // statements
      break;
    case 9: // ...
      // statements
      break;
    case 11:
      // statements
      break;
  
    default:
      // statements
      break;
  }
  
  bool break_flag = true;
  return true;
}
