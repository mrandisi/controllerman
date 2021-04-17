/**
 * View_Cyan
 */

#include "Arduino.h"
#include "View.h"
#include "View_Cyan.h"

View_Cyan::View_Cyan()
{
  if(submit==NULL) {
    submit = new Submit_gtk();
  }
  
  uint8_t cyan[] = {0,0,255};
  this->setColor(cyan);
}

void View_Cyan::butt1_singlePress() {
  submit->toggleFx(21);
}
void View_Cyan::butt2_singlePress() {
  submit->toggleFx(22);
}
void View_Cyan::butt3_singlePress() {
  submit->toggleFx(23);
}
void View_Cyan::butt4_singlePress() {
  submit->toggleFx(24);
}
void View_Cyan::butt5_singlePress() {
  submit->toggleFx(25);
}
void View_Cyan::butt6_singlePress() {
  submit->toggleFx(26);
}
