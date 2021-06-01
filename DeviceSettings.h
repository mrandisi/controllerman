#ifndef DEVICE_SETTINGS_H
#define DEVICE_SETTINGS_H

#include <Arduino.h>
#include <EEPROM.h>

// midi settings
uint8_t channel = 8; // channel: OFF / 1-16

uint8_t CURRENT_VIEW = 0;
uint8_t PATCH_STATE = 1; // PC: OFF / 1-128 -> path from 1 to 125
bool button_states[24];

// these are the default effect on/off states at the patch loading
//bool default_states[24];

// button-to-device association (array index to index)
uint8_t button2deviceMap[24] = 
                      {0, 1, 2, 3, 26, 6,           // effects on view 1
                        11, 12, 13, 14, 15, 1,      // effects on view 2
                          1, 2, 3, 4, 5, 0,         // effects on view 3
                            28, 16, 13, 8, 15, 19};  // effects on view 4

// the ControlNumber associated to the gtk modules
uint8_t ccs[31]    =  {		
          11, 12, 13, 14, 15, 
					16, 17, 18, 19, 20, 
					21, 22, 23, 24, 25, 
					26, 27, 28, 29, 30, 
					31, 32, 33, 34, 35, 
					36, 37, 38, 39, 40,
					41};

              
// Associations between FX modiles and his effect name, array index (from 0)
/*uint8_t module2fx1_association=6;
uint8_t module2fx2_association=12;
uint8_t module2fx3_association=30;*/


/*struct Effect {
  uint8_t button;
  boolean default_state;
  uint8_t ccNumber;
  char* shortName[8];
  char* longName[16];
};*/

namespace settings
{
  
} // end namespace setings

const char gtModulesShortName_0[] PROGMEM = "PFX";
const char gtModulesShortName_1[] PROGMEM = "CMP";
const char gtModulesShortName_2[] PROGMEM = "EQ3";
const char gtModulesShortName_3[] PROGMEM = "Dv1";
const char gtModulesShortName_4[] PROGMEM = "DS1";
const char gtModulesShortName_5[] PROGMEM = "SR1";
const char gtModulesShortName_6[] PROGMEM = "AP1";
const char gtModulesShortName_7[] PROGMEM = "NS1";
const char gtModulesShortName_8[] PROGMEM = "EQ1";
const char gtModulesShortName_9[] PROGMEM = "DS2";
const char gtModulesShortName_10[] PROGMEM = "SR2";
const char gtModulesShortName_11[] PROGMEM = "AP2";
const char gtModulesShortName_12[] PROGMEM = "NS2";
const char gtModulesShortName_13[] PROGMEM = "EQ2";
const char gtModulesShortName_14[] PROGMEM = "MX1";
const char gtModulesShortName_15[] PROGMEM = "FX1";
const char gtModulesShortName_16[] PROGMEM = "EQ4";
const char gtModulesShortName_17[] PROGMEM = "FV";
const char gtModulesShortName_18[] PROGMEM = "DL1";
const char gtModulesShortName_19[] PROGMEM = "DL2";
const char gtModulesShortName_20[] PROGMEM = "DL3";
const char gtModulesShortName_21[] PROGMEM = "DL4";
const char gtModulesShortName_22[] PROGMEM = "MDL";
const char gtModulesShortName_23[] PROGMEM = "FX2";
const char gtModulesShortName_24[] PROGMEM = "FX3";
const char gtModulesShortName_25[] PROGMEM = "CHO";
const char gtModulesShortName_26[] PROGMEM = "RVB";
const char gtModulesShortName_27[] PROGMEM = "DV2";
const char gtModulesShortName_28[] PROGMEM = "MX2";
const char gtModulesShortName_29[] PROGMEM = "DV3";
const char gtModulesShortName_30[] PROGMEM = "MX3";

const char* const gtModulesShortName[] PROGMEM = {
  gtModulesShortName_0,gtModulesShortName_1,gtModulesShortName_2,gtModulesShortName_3,gtModulesShortName_4,
  gtModulesShortName_5,gtModulesShortName_6,gtModulesShortName_7,gtModulesShortName_8,gtModulesShortName_9,
  gtModulesShortName_10,gtModulesShortName_11,gtModulesShortName_12,gtModulesShortName_13,gtModulesShortName_14,
  gtModulesShortName_15,gtModulesShortName_16,gtModulesShortName_17,gtModulesShortName_18,gtModulesShortName_19,
  gtModulesShortName_20,gtModulesShortName_21,gtModulesShortName_22,gtModulesShortName_23,gtModulesShortName_24,
  gtModulesShortName_25,gtModulesShortName_26,gtModulesShortName_27,gtModulesShortName_28,gtModulesShortName_29,
  gtModulesShortName_30};

const char gtModuleLongName_0[] PROGMEM = "Pedal FX";
const char gtModuleLongName_1[] PROGMEM = "Compressor";
const char gtModuleLongName_2[] PROGMEM = "Equalizer 3";
const char gtModuleLongName_3[] PROGMEM = "Divider 1";
const char gtModuleLongName_4[] PROGMEM = "Distors 1";
const char gtModuleLongName_5[] PROGMEM = "Send/Ret 1";
const char gtModuleLongName_6[] PROGMEM = "AIRD Pre 1";
const char gtModuleLongName_7[] PROGMEM = "Noise Supp 1";
const char gtModuleLongName_8[] PROGMEM = "Equal. 1";
const char gtModuleLongName_9[] PROGMEM = "Distors 2"; 
const char gtModuleLongName_10[] PROGMEM = "Send/Ret 2";
const char gtModuleLongName_11[] PROGMEM = "AIRD Pre 2";
const char gtModuleLongName_12[] PROGMEM = "Noise Supp 2";
const char gtModuleLongName_13[] PROGMEM = "Equalizer 2";
const char gtModuleLongName_14[] PROGMEM = "Mixer 1";
const char gtModuleLongName_15[] PROGMEM = "Effect 1";
const char gtModuleLongName_16[] PROGMEM = "Equalizer 4";
const char gtModuleLongName_17[] PROGMEM = "Foot Vol";
const char gtModuleLongName_18[] PROGMEM = "Delay 1";
const char gtModuleLongName_19[] PROGMEM = "Delay 2";
const char gtModuleLongName_20[] PROGMEM = "Delay 3";
const char gtModuleLongName_21[] PROGMEM = "Delay 4";
const char gtModuleLongName_22[] PROGMEM = "Mast Delay";
const char gtModuleLongName_23[] PROGMEM = "Effect 2";
const char gtModuleLongName_24[] PROGMEM = "Effect 3";
const char gtModuleLongName_25[] PROGMEM = "Chorus";
const char gtModuleLongName_26[] PROGMEM = "Reverb";
const char gtModuleLongName_27[] PROGMEM = "Divider 2";
const char gtModuleLongName_28[] PROGMEM = "Mixer 2";
const char gtModuleLongName_29[] PROGMEM = "Divider 3";
const char gtModuleLongName_30[] PROGMEM = "Mixer 3";

const char* const gtModulesLongName[] PROGMEM = {
  gtModuleLongName_0,gtModuleLongName_1,gtModuleLongName_2,gtModuleLongName_3,gtModuleLongName_4,
  gtModuleLongName_5,gtModuleLongName_6,gtModuleLongName_7,gtModuleLongName_8,gtModuleLongName_9,
  gtModuleLongName_10,gtModuleLongName_11,gtModuleLongName_12,gtModuleLongName_13,gtModuleLongName_14,
  gtModuleLongName_15,gtModuleLongName_16,gtModuleLongName_17,gtModuleLongName_18,gtModuleLongName_19,
  gtModuleLongName_20,gtModuleLongName_21,gtModuleLongName_22,gtModuleLongName_23,gtModuleLongName_24,
  gtModuleLongName_25,gtModuleLongName_26,gtModuleLongName_27,gtModuleLongName_28,gtModuleLongName_29,
  gtModuleLongName_30};

/*const char gtModFxName_0[] PROGMEM = "AC. G Sim";
const char gtModFxName_1[] PROGMEM = "AC Reso";
const char gtModFxName_2[] PROGMEM = "Auto Wah";
const char gtModFxName_3[] PROGMEM = "Chorus";
const char gtModFxName_4[] PROGMEM = "CHO Bass";
const char gtModFxName_5[] PROGMEM = "C-Vibe";
const char gtModFxName_6[] PROGMEM = "Comp";
const char gtModFxName_7[] PROGMEM = "Defretr";
const char gtModFxName_8[] PROGMEM = "Defret B";
const char gtModFxName_9[] PROGMEM = "Dist";
const char gtModFxName_10[] PROGMEM = "Feedbakr";
const char gtModFxName_11[] PROGMEM = "Flanger";
const char gtModFxName_12[] PROGMEM = "Flangr B";
const char gtModFxName_13[] PROGMEM = "Harmonst";
const char gtModFxName_14[] PROGMEM = "Humanizr";
const char gtModFxName_15[] PROGMEM = "Octave";
const char gtModFxName_16[] PROGMEM = "Oct Bass";
const char gtModFxName_17[] PROGMEM = "Overtone";
const char gtModFxName_18[] PROGMEM = "Pan";
const char gtModFxName_19[] PROGMEM = "Phazer";
const char gtModFxName_20[] PROGMEM = "PITCH SFT";
const char gtModFxName_21[] PROGMEM = "Ring Mod";
const char gtModFxName_22[] PROGMEM = "Rotary";
const char gtModFxName_23[] PROGMEM = "Sitar Sim";
const char gtModFxName_24[] PROGMEM = "Slicer";
const char gtModFxName_25[] PROGMEM = "SLW Gear";
const char gtModFxName_26[] PROGMEM = "SG Bass";
const char gtModFxName_27[] PROGMEM = "SND Hold";
const char gtModFxName_28[] PROGMEM = "S-Bend";
const char gtModFxName_29[] PROGMEM = "Touch WH";
const char gtModFxName_30[] PROGMEM = "TW Bass";
const char gtModFxName_31[] PROGMEM = "Tremolo";
const char gtModFxName_32[] PROGMEM = "Vibrato";

const char* const gtModFxName[] PROGMEM = {
gtModFxName_0,gtModFxName_1,gtModFxName_2,gtModFxName_3,gtModFxName_4,
gtModFxName_5,gtModFxName_6,gtModFxName_7,gtModFxName_8,gtModFxName_9,
gtModFxName_10,gtModFxName_11,gtModFxName_12,gtModFxName_13,gtModFxName_14,
gtModFxName_15,gtModFxName_16,gtModFxName_17,gtModFxName_18,gtModFxName_19,
gtModFxName_20,gtModFxName_21,gtModFxName_22,gtModFxName_23,gtModFxName_24,
gtModFxName_25,gtModFxName_26,gtModFxName_27,gtModFxName_28,gtModFxName_29,
gtModFxName_30,gtModFxName_31,gtModFxName_32};
*/


 byte boolArrayToByte(bool boolArray[8]) {
  byte bits;
  
  for(byte i=0; i<8; i++) {
    bitWrite(bits, i, boolArray[i]);
  }
  return bits;
}

void byteToBoolArray(byte eightBit, bool b[]) {
  
  for(byte i=0; i<8; i++) {
    b[i] = bitRead(eightBit, i);
  }
}

void load_default_states() {
  int initial_location=0;
  
  bool boolArr[3][8];
  
  int first_location = initial_location + (PATCH_STATE-1) * 4;
  
  byte newByte;

  // load byte
  for(uint8_t i=0; i<3; i++) {
    newByte = EEPROM.read(first_location+i);
    
    // convert to array
    byteToBoolArray(newByte, boolArr[i]);
  }
  
  uint8_t k=0;
  for(uint8_t i=0; i<3; i++) {
    //Serial.print("+");
    for(uint8_t j=0; j<8; j++) {
      button_states[k] = boolArr[i][j];
      k++;
      //Serial.print(boolArr[i][j], BIN);
    }
    //Serial.println();
  } // end for ij-k
}

void write_button_states() {
  int initial_location=0;
  bool boolArr[3][8];
  
  int first_location = initial_location + (PATCH_STATE-1) * 4;
  
  uint8_t k=0;
  for(uint8_t i=0; i<3; i++) {
    for(uint8_t j=0; j<8; j++) {
      boolArr[i][j] = button_states[k];
      k++;
    }
  } // end for ij-k
  
  // Write
  for(uint8_t i=0; i<3; i++) {
    byte binByte = boolArrayToByte(boolArr[i]);
    //Serial.print(":");
    //Serial.println(binByte, BIN);
    EEPROM.write(first_location+i, binByte);
  }

}

void init_settings() {

  load_default_states();
  
}




#endif /* DEVICE_SETTINGS_H */
