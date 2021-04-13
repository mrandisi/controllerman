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
