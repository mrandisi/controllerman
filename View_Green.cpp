/**
 * View_Green
 */

#include "Arduino.h"
#include "View.h"
#include "View_Green.h"

View_Green::View_Green()
{
  if(submit==NULL) {
    submit = new Submit_gtk();
  }
  
  uint8_t green[] = {0,255,0};
  this->setColor(green);
}

void View_Green::butt1_singlePress() {
  Serial.println("green butt1 single press");
}
void View_Green::butt2_singlePress() {
  Serial.println("green butt2 single press");
}
void View_Green::butt3_singlePress() {
  
  submit->patchUp();
  Serial.println("green butt3 single press");
}
void View_Green::butt4_singlePress() {
  Serial.println("green butt4 single press");
}
void View_Green::butt5_singlePress() {
  Serial.println("green butt5 single press");
}
void View_Green::butt6_singlePress() {
  
  submit->patchDown();
  Serial.println("green butt6 single press");
}

void View_Green::doublePress_1_2() {
  Serial.println("green double press 1+2");
}
void View_Green::doublePress_2_3() {
  Serial.println("green double press 2+3");
}
void View_Green::doublePress_4_5() {
  Serial.println("green double press 4+5");
}
void View_Green::doublePress_5_6() {
  Serial.println("green double press 5+6");
}
