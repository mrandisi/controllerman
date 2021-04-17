/**
 * View
 */
#ifndef Submit_gtk_h
#define Submit_gtk_h

#include "Arduino.h"

class Submit_gtk
{
  public:
    Submit_gtk();
    
    void patchUp();
    void patchDown();
    
  private:
    uint8_t channel = 8; // channel: OFF / 1-16
    uint8_t patch_state = 1; // PC: OFF / 1-128 -> path from 1 to 125
    
    
};

#endif
