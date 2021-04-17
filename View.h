/**
 * View
 */
#ifndef View_h
#define View_h

#include "Arduino.h"
#include "Submit_gtk.h"

class View
{
  public:
    View();
    bool DEBUG=true;
    void setColor(uint8_t []);
    void modifyColor(uint8_t []);
    
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
    virtual bool butt1_singlePress_hold();
    virtual bool butt2_singlePress_hold();
    virtual bool butt3_singlePress_hold();
    virtual bool butt4_singlePress_hold();
    virtual bool butt5_singlePress_hold();
    virtual bool butt6_singlePress_hold();

    virtual void doublePress_1_2();
    virtual void doublePress_2_3();
    virtual void doublePress_4_5();
    virtual void doublePress_5_6();
    virtual bool doublePress_hold_1_2();
    virtual bool doublePress_hold_2_3();
    virtual bool doublePress_hold_4_5();
    virtual bool doublePress_hold_5_6();
    
    Submit_gtk* submit = NULL;
    
  protected:
    void printSerial(char*, int);
    
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    
    
  private:
    
    
};

#endif
