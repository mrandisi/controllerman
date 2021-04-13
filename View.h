/**
 * View
 */
#ifndef View_h
#define View_h

#include "Arduino.h"

class View
{
  public:
    View();
    bool DEBUG=true;
    uint8_t getColor();
    uint8_t getRed();
    uint8_t getGreen();
    uint8_t getBlue();
    
    void singlePress(uint8_t);
    bool singlePress_hold(uint8_t);
    void doublePress(uint8_t, uint8_t);
    bool doublePress_hold(uint8_t, uint8_t);
    
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
    
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    
  private:
    
};

#endif
