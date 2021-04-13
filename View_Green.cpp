/**
 * View_Green
 */

#include "Arduino.h"
#include "View.h"
#include "View_Green.h"

View_Green::View_Green()
{
    this->red=0;
    this->green=255;
    this->blue=0;

    //uint8_t col[3] = {255,0,0};
    // *this->color=*col;
}

void View_Green::butt1_singlePress() {
  Serial.println("green butt1 single press");
}
void View_Green::butt2_singlePress() {
  Serial.println("green butt2 single press");
}
void View_Green::butt3_singlePress() {
  Serial.println("green butt3 single press");
}
void View_Green::butt4_singlePress() {
  Serial.println("green butt4 single press");
}
void View_Green::butt5_singlePress() {
  Serial.println("green butt5 single press");
}
void View_Green::butt6_singlePress() {
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
