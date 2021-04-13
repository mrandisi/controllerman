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
