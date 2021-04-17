/**
 * View_Orange
 */

#include "Arduino.h"
#include "View.h"
#include "View_Orange.h"

View_Orange::View_Orange()
{
  if(submit==NULL) {
    submit = new Submit_gtk();
  }
  
  uint8_t orange[] = {255,0,0};
  this->setColor(orange);
}

void View_Orange::butt1_singlePress() {
  submit->toggleFx(11);
}
void View_Orange::butt2_singlePress() {
  submit->toggleFx(12);
}
void View_Orange::butt3_singlePress() {
  submit->toggleFx(13);
}
void View_Orange::butt4_singlePress() {
  submit->toggleFx(14);
}
void View_Orange::butt5_singlePress() {
  submit->toggleFx(15);
}
void View_Orange::butt6_singlePress() {
  submit->toggleFx(16);
}
