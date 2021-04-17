/**
 * GreenView
 */

#include "Arduino.h"
#include "View.h"
//#include "Submit_gtk.h"

View::View()
{
  
}
void View::modifyColor(uint8_t col[]) {
  col[0] = this->red;
  col[1] = this->green;
  col[2] = this->blue;
}
void View::setColor(uint8_t col[]) {
  this->red = col[0];
  this->green = col[1];
  this->blue = col[2];
}

void View::singlePress(uint8_t button) {
  if(DEBUG) {
    Serial.print("singlePress: ");
    Serial.println(button, DEC);
  }
  switch (button) {
    case 1:
      butt1_singlePress();
      break;
    case 2:
      butt2_singlePress();
      break;
    case 3:
      butt3_singlePress();
      break;
    case 4:
      butt4_singlePress();
      break;
    case 5:
      butt5_singlePress();
      break;
    case 6:
      butt6_singlePress();
      break;
  
    default:
      // statements
      break;
  }
}
bool View::singlePress_hold(uint8_t button) {
  if(DEBUG) {
    Serial.print("singlePress_hold: ");
    Serial.println(button, DEC);
  }
  switch (button) {
    case 1:
      // statements
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
    Serial.print("doublePress: ");
    Serial.print(button1, DEC);
    Serial.println(button2, DEC);
  }
  uint8_t c = button1+button2;
  switch (c) {
    case 3:
      doublePress_1_2();
      break;
    case 5:
      doublePress_2_3();
      break;
    case 9:
      doublePress_4_5();
      break;
    case 11:
      doublePress_5_6();
      break;
  
    default:
      // statements
      break;
  }
}
bool View::doublePress_hold(uint8_t button1, uint8_t button2) {
  if(DEBUG) {
    Serial.print("doublePress_hold: ");
    Serial.print(button1, DEC);
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
