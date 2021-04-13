/**
 * View_Cyan
 */

#include "Arduino.h"
#include "View.h"
#include "View_Cyan.h"

View_Cyan::View_Cyan()
{
    this->red=0;
    this->green=255;
    this->blue=255;

    //uint8_t col[3] = {255,0,0};
    // *this->color=*col;
}

void View_Cyan::butt1_singlePress() {
  Serial.println("cyan butt1 single press");
}
void View_Cyan::butt2_singlePress() {
  Serial.println("cyan butt2 single press");
}
void View_Cyan::butt3_singlePress() {
  Serial.println("cyan butt3 single press");
}
void View_Cyan::butt4_singlePress() {
  Serial.println("cyan butt4 single press");
}
void View_Cyan::butt5_singlePress() {
  Serial.println("cyan butt5 single press");
}
void View_Cyan::butt6_singlePress() {
  Serial.println("cyan butt6 single press");
}

void View_Cyan::doublePress_1_2() {
  Serial.println("cyan double press 1+2");
}
void View_Cyan::doublePress_2_3() {
  Serial.println("cyan double press 2+3");
}
void View_Cyan::doublePress_4_5() {
  Serial.println("cyan double press 4+5");
}
void View_Cyan::doublePress_5_6() {
  Serial.println("cyan double press 5+6");
}
