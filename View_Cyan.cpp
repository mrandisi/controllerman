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
