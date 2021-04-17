/**
 * View_Orange
 */
#ifndef View_Orange_h
#define View_Orange_h

#include "Arduino.h"
#include "View.h"

class View_Orange : public View
{
  public:
    View_Orange();

    virtual void butt1_singlePress();
    virtual void butt2_singlePress();
    virtual void butt3_singlePress();
    virtual void butt4_singlePress();
    virtual void butt5_singlePress();
    virtual void butt6_singlePress();
    
  private:
    
};

#endif
