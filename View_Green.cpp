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
  submit->toggleFx(1);
}
void View_Green::butt2_singlePress() {
  submit->toggleFx(2);
}
void View_Green::butt3_singlePress() {
  submit->toggleFx(3);
}
void View_Green::butt4_singlePress() {
  submit->toggleFx(4);
}
void View_Green::butt5_singlePress() {
  submit->toggleFx(5);
}
void View_Green::butt6_singlePress() {
  submit->toggleFx(6);
}
