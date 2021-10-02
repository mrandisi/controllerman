#ifndef DEVICE_SETTINGS_H
#define DEVICE_SETTINGS_H

#include <Arduino.h>
#include <EEPROM.h>

// midi settings
uint8_t CHANNEL = 8; // channel: OFF / 1-16

uint8_t CURRENT_LAYOUT = 0;
char LAYOUT_TITLE[16];
uint8_t PATCH_STATE = 1; // PC: OFF / 1-128 -> path from 1 to 125
bool BUTTON_STATES[24];

int EEPROM_STATES_SHIFT=120;
int EEPROM_FX_SHIFT=520;
const int EEPROM_PATCH_DEFAULT_STATE=40;
const int EEPROM_OUTCHANNEL=51;

namespace settings
{
  
} // end namespace setings

/*const char defFx_0[] PROGMEM = "AC AmpChannel";
const char defFx_1[] PROGMEM = "CMPCompressor";
const char defFx_2[] PROGMEM = "WAHPedal";
const char defFx_3[] PROGMEM = "BSTBoost";
const char defFx_4[] PROGMEM = "ECOEcho";
const char defFx_5[] PROGMEM = "RVBReverb";
const char defFx_6[] PROGMEM = "CHOChorus";
const char defFx_7[] PROGMEM = "FLRFlanger";
const char defFx_8[] PROGMEM = "TRMTremolo";
const char defFx_9[] PROGMEM = "PHAPhazer";
const char defFx_10[] PROGMEM = "DLYDelay";
const char defFx_11[] PROGMEM = "EQZEqualizer";
const char defFx_12[] PROGMEM = "PITPitchSh";
const char defFx_13[] PROGMEM = "OVDOverdrive";
const char defFx_14[] PROGMEM = "DSTDistorsion";
const char defFx_15[] PROGMEM = "OCTOctaver";
const char defFx_16[] PROGMEM = "VBTVibrato";
const char defFx_17[] PROGMEM = "FVLFoot Vol";
const char defFx_18[] PROGMEM = "FUZFuzz";
const char defFx_19[] PROGMEM = "RTRRotary";
const char defFx_20[] PROGMEM = "NSPNoiseSuppr";
const char defFx_21[] PROGMEM = "SBKSlapback";
const char defFx_22[] PROGMEM = "AWHAutoWah";
const char defFx_23[] PROGMEM = "ENVEnvpFilter";
const char* const defFx[] PROGMEM = {
  defFx_0,defFx_1,defFx_2,defFx_3,defFx_4,
  defFx_5,defFx_6,defFx_7,defFx_8,defFx_9,
  defFx_10,defFx_11,defFx_12,defFx_13,defFx_14,
  defFx_15,defFx_16,defFx_17,defFx_18,defFx_19,
  defFx_20,defFx_21,defFx_22,defFx_23
};*/


const char layoutName_0[] PROGMEM = "Red";
const char layoutName_1[] PROGMEM = "Cyan";
const char layoutName_2[] PROGMEM = "Green";
const char layoutName_3[] PROGMEM = "Purple";
const char* const layoutName[] PROGMEM = {layoutName_0,layoutName_1,layoutName_2,layoutName_3};

const char shortLayoutName_0[] PROGMEM = "RED";
const char shortLayoutName_1[] PROGMEM = "CYA";
const char shortLayoutName_2[] PROGMEM = "GRN";
const char shortLayoutName_3[] PROGMEM = "PUR";
const char* const shortLayoutName[] PROGMEM = {shortLayoutName_0,shortLayoutName_1,shortLayoutName_2,shortLayoutName_3};

const char menu1_0[] PROGMEM = "Save button states";
const char menu1_1[] PROGMEM = "Swap Buttons";
const char menu1_2[] PROGMEM = "Edit button";
const char menu1_3[] PROGMEM = "Set boot patch";
const char menu1_4[] PROGMEM = "Set out channel";
const char menu1_5[] PROGMEM = "Reset device";
const char* const menu1[] PROGMEM = {menu1_0, menu1_1, menu1_2, menu1_3, menu1_4, menu1_5};

const char property_0[] PROGMEM = "Settings";
const char property_1[] PROGMEM = "Choose 1st button";
const char property_2[] PROGMEM = "Choose 2nd button";
const char property_3[] PROGMEM = "Choose button";
const char property_4[] PROGMEM = "Select boot patch";
const char property_5[] PROGMEM = "Out channel";
const char property_6[] PROGMEM = "Are you shure?";
const char property_7[] PROGMEM = "Reset, wait...?";
const char* const property[] PROGMEM = {property_0, property_1, property_2, property_3, property_4, property_5, property_6, property_7};

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

void load_default_states(uint8_t patch) {
  bool boolArr[3][8];
  
  int first_location = EEPROM_STATES_SHIFT + (patch-1) * 3;
  
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
      BUTTON_STATES[k] = boolArr[i][j];
      k++;
      //Serial.print(boolArr[i][j], BIN);
    }
    //Serial.println();
  } // end for ij-k
}

void write_default_states(uint8_t patch) {
  bool boolArr[3][8];
  
  int first_location = EEPROM_STATES_SHIFT + (patch-1) * 3;
  
  uint8_t k=0;
  for(uint8_t i=0; i<3; i++) {
    for(uint8_t j=0; j<8; j++) {
      boolArr[i][j] = BUTTON_STATES[k];
      k++;
    }
  } // end for ij-k
  
  // Write
  for(uint8_t i=0; i<3; i++) {
    byte binByte = boolArrayToByte(boolArr[i]);
    EEPROM.write(first_location+i, binByte);
  }
}

void clear_eeprom() {
  bool a[8] = {0,0,0,0,0,0,0,0};
  byte emptyByte = boolArrayToByte(a);
  for(int i=0; i<1024; i++) {
    EEPROM.write(EEPROM_STATES_SHIFT+i, emptyByte);
  }
}

void write_default_fx() {
  
  char fxName[13];
  
  // Write
  for(uint8_t i=0; i<24; i++) {
    //strcpy_P(fxName, (char*)pgm_read_word(&( defFx[i] ))); // get the couple short+long_name
    sprintf(fxName, "E%02dEffect %d", i+1, i+1);

    int charLocation = EEPROM_FX_SHIFT + (i * 16);  // +0 first location
    EEPROM.write(charLocation, 0);  // long press disabled
    EEPROM.write(charLocation+1, 0);  // double click disabled
    EEPROM.write(charLocation+2, i + 1);  // write consecutive cc numbers from 1 to 24
    
    for(uint8_t j=3; j<16; j++) {
      
      int charLocation = EEPROM_FX_SHIFT + (i * 16) + j;
      
      EEPROM.write(charLocation, fxName[j-3]);
    }
    strcpy(fxName, "             ");
  }
}

uint8_t getFxLongPressLink(uint8_t virtualButton) { 
  return EEPROM.read(EEPROM_FX_SHIFT + (virtualButton*16));
}

uint8_t getFxDoubleClickLink(uint8_t virtualButton) {
  return EEPROM.read(EEPROM_FX_SHIFT+1 + (virtualButton*16));
}

uint8_t getFxCCnum(uint8_t virtualButton) {
  return EEPROM.read(EEPROM_FX_SHIFT+2 + (virtualButton*16));
  
}

void getFxShortName(uint8_t virtualButton, char * fx) { // button from 0 to 23
  //strcpy(fx, "   ");
  
  for(uint8_t i=3; i<6; i++) {
    fx[i-3] = EEPROM.read(EEPROM_FX_SHIFT + (virtualButton*16) + i);
  }
  fx[3]='\0';
}

void getFxLongName(uint8_t virtualButton, char fx[10]) { // button from 0 to 23
  //strcpy(fx, "          ");
  for(uint8_t i=6; i<16; i++) {
    fx[i-6] = EEPROM.read(EEPROM_FX_SHIFT + (virtualButton*16) + i);
  }
  fx[10]='\0';
}

void getFx(uint8_t virtualButton, char fx[17]) {   // button from 0 to 23
  
  for(uint8_t i=0; i<16; i++) {
    fx[i] = EEPROM.read(EEPROM_FX_SHIFT + (virtualButton*16) + i);
  }
  fx[16]='\0';
}

void writeFx(uint8_t virtualButton, char fx[17]) {   // button from 0 to 23
  
  for(uint8_t i=0; i<16; i++) {
    EEPROM.write(EEPROM_FX_SHIFT + (virtualButton*16) + i, fx[i]);
  }
  
}

void setFxCCnum(uint8_t virtualButton, byte ccnum) {
  EEPROM.write(EEPROM_FX_SHIFT + (virtualButton*16)+2, ccnum);
}

void writeFxShortName(uint8_t virtualButton, char fx[]) { // button from 0 to 23
  
  for(uint8_t i=3; i<6; i++) {
    EEPROM.write(EEPROM_FX_SHIFT + (virtualButton*16) + i, fx[i-3]);
  }
}

void setFxLongName(uint8_t virtualButton, char fx[10]) { // button from 0 to 23
  
  for(uint8_t i=6; i<16; i++) {
    EEPROM.write(EEPROM_FX_SHIFT + (virtualButton*16) + i, fx[i-6]);
  }
}

uint8_t read_default_patch() {
  uint8_t patch = EEPROM.read(EEPROM_PATCH_DEFAULT_STATE);
  if(patch > 0)
    return patch;
  return 1;
}

void write_default_patch(uint8_t patch_num) {
  EEPROM.write(EEPROM_PATCH_DEFAULT_STATE, patch_num);
}

uint8_t read_outChannel() {
  uint8_t channel = EEPROM.read(EEPROM_OUTCHANNEL);
  if(channel > 0)
    return channel;
  return 1;
}

void write_outChannel(uint8_t channel) {
  EEPROM.write(EEPROM_OUTCHANNEL, channel);
}

// Reboot
void(* reboot)(void) = 0;

void reset_device() {
    clear_eeprom();
    EEPROM.write(0, 87);  // flag for firstRun (random number). So, if == 87 device is already formatted.
    write_outChannel(8);
    write_default_patch(1);
    write_default_fx();
    reboot();
}

bool isFirstRun() {
  uint8_t code = EEPROM.read(0);
  if(code == 87)
    return false;
   else
    return true;
}

void getPMArrayVal(uint8_t target, uint8_t index, char myString[20]) {
  
  if(target == 1) {
    strcpy_P(myString, (char*)pgm_read_word(&( menu1[index] )));
  } else if(target == 3) {  // list of virtual buttons
          uint8_t vButt = index+1;
          uint8_t layout_n = (vButt-1) / 6;
          uint8_t button = vButt - (layout_n*6);
          char lsName[4];
          strcpy_P(lsName, (char*)pgm_read_word(&( shortLayoutName[layout_n] )));
          char shortName[4];
          getFxShortName(index, shortName);
          sprintf(myString, "%s-%d %s", lsName, vButt, shortName);
    
  } else if(target == 4) {
    sprintf(myString, "%03d", index+1);
  } else if(target == 5) {
    sprintf(myString, "%d", index+1);
  } else if(target == 10) {
    strcpy_P(myString, (char*)pgm_read_word(&( property[index] )));
  }
}

void getProperty(uint8_t index, char myString[20]) {
  getPMArrayVal(/*target*/10, index, myString);
}

uint8_t getPMStrSize(uint8_t target) {
  
  if(target==1) {
    return 6;
  } else if(target == 3) {
    return 24;
  } else if(target == 4) {
    return 125;
  } else if(target == 5) {
    return 16;
  }
  return 0;
}

#endif /* DEVICE_SETTINGS_H */