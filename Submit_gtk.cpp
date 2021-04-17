/**
 * Submit for Boss gt1000
 */

#include "LibMIDI/MIDI.h"
#include "Arduino.h"
#include "Submit_gtk.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

Submit_gtk::Submit_gtk()
{
  //Serial1.begin(9600);
  MIDI.begin();
}

/**
 * Increment the patch number
 */
void Submit_gtk::patchUp(){
  if(patch_state < 125) { // 125 is the last patch, 126 and 127 are not used.
    patch_state++;
  } else {
    patch_state=1;  // loops to the first patch
  }
  
  MIDI.sendProgramChange (patch_state-1, channel); // counts from 0 that stands for the patch 1
}

/**
 * Decrement the patch number
 */
void Submit_gtk::patchDown() {
  if(patch_state > 1) {
    patch_state--;
  } else {
    patch_state=125;  // loops to the last patch
  }
  
  MIDI.sendProgramChange (patch_state-1, channel); // counts from 0 that stands for the patch 1
}

void Submit_gtk::toggleFx(uint8_t cc) {
  MIDI.sendControlChange(cc, 0, channel);
  delay(5);
  MIDI.sendControlChange(cc, 127, channel);
}
