/**
 * View_Orange
 */

#include "Arduino.h"
#include "View.h"
#include "View_Orange.h"

View_Orange::View_Orange()
{
    this->red=255;
    this->green=127;
    this->blue=0;

    //uint8_t col[3] = {255,0,0};
    // *this->color=*col;
}

void View_Orange::butt1_singlePress() {
  Serial.println("orange butt1 single press");
}
void View_Orange::butt2_singlePress() {
  Serial.println("orange butt2 single press");
}
void View_Orange::butt3_singlePress() {
  Serial.println("orange butt3 single press");
}
void View_Orange::butt4_singlePress() {
  Serial.println("orange butt4 single press");
}
void View_Orange::butt5_singlePress() {
  Serial.println("orange butt5 single press");
}
void View_Orange::butt6_singlePress() {
  Serial.println("orange butt6 single press");
}

void View_Orange::doublePress_1_2() {
  Serial.println("orange double press 1+2");
}
void View_Orange::doublePress_2_3() {
  Serial.println("orange double press 2+3");
}
void View_Orange::doublePress_4_5() {
  Serial.println("orange double press 4+5");
}
void View_Orange::doublePress_5_6() {
  Serial.println("orange double press 5+6");
}
