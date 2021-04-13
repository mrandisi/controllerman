/**
 * View_Green
 */
#ifndef View_Green_h
#define View_Green_h

#include "Arduino.h"
#include "View.h"

class View_Green : public View
{
  public:
    View_Green();

    virtual void butt1_singlePress();
    virtual void butt2_singlePress();
    virtual void butt3_singlePress();
    virtual void butt4_singlePress();
    virtual void butt5_singlePress();
    virtual void butt6_singlePress();

    virtual void doublePress_1_2();
    virtual void doublePress_2_3();
    virtual void doublePress_4_5();
    virtual void doublePress_5_6();
    
  private:
    
};

#endif
