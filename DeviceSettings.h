#ifndef DEVICE_SETTINGS_H
#define DEVICE_SETTINGS_H

#include <Arduino.h>
#include <EEPROM.h>

// midi settings
uint8_t channel = 8; // channel: OFF / 1-16

uint8_t CURRENT_LAYOUT = 0;
char LAYOUT_TITLE[16];
uint8_t PATCH_STATE = 1; // PC: OFF / 1-128 -> path from 1 to 125
bool button_states[24];

int eeprom_states_shift=100;
int eeprom_fx_shift=500;

// these are the default effect on/off states at the patch loading
//bool default_states[24];

// button-to-device association (array index to index)
uint8_t button2deviceMap[24] = 
                      {0, 1, 2, 3, 26, 6,           // effects on view 1
                        11, 12, 13, 14, 15, 1,      // effects on view 2
                          1, 2, 3, 4, 5, 0,         // effects on view 3
                            28, 16, 13, 8, 15, 19};  // effects on view 4

// the ControlNumber associated to the gtk modules
/*uint8_t ccs[31]    =  {		
          11, 12, 13, 14, 15, 
					16, 17, 18, 19, 20, 
					21, 22, 23, 24, 25, 
					26, 27, 28, 29, 30, 
					31, 32, 33, 34, 35, 
					36, 37, 38, 39, 40,
					41};*/

              
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


const char defFx_0[] PROGMEM = "EQZEqualizer";
const char defFx_1[] PROGMEM = "WAHPedal";
const char defFx_2[] PROGMEM = "AWHAutoWah";
const char defFx_3[] PROGMEM = "ENVEnvpFilter";
const char defFx_4[] PROGMEM = "CMPCompressor";
const char defFx_5[] PROGMEM = "BSTBoost";
const char defFx_6[] PROGMEM = "OVDOverdrive";
const char defFx_7[] PROGMEM = "FUZFuzz";
const char defFx_8[] PROGMEM = "DSTDistorsion";
const char defFx_9[] PROGMEM = "AC AmpChannel";
const char defFx_10[] PROGMEM = "NSPNoiseSuppr";
const char defFx_11[] PROGMEM = "OCTOctaver";
const char defFx_12[] PROGMEM = "PITPitchSh";
const char defFx_13[] PROGMEM = "CHOChorus";
const char defFx_14[] PROGMEM = "FLRFlanger";
const char defFx_15[] PROGMEM = "PHAPhazer";
const char defFx_16[] PROGMEM = "VBTVibrato";
const char defFx_17[] PROGMEM = "FVLFoot Vol";
const char defFx_18[] PROGMEM = "TRMTremolo";
const char defFx_19[] PROGMEM = "RTRRotary";
const char defFx_20[] PROGMEM = "DLYDelay";
const char defFx_21[] PROGMEM = "SBKSlapback";
const char defFx_22[] PROGMEM = "ECOEcho";
const char defFx_23[] PROGMEM = "RVBReverb";

/*const char defFx_0[] PROGMEM = "1  A";
const char defFx_1[] PROGMEM = "2  B";
const char defFx_2[] PROGMEM = "3  C";
const char defFx_3[] PROGMEM = "4  D";
const char defFx_4[] PROGMEM = "5  E";
const char defFx_5[] PROGMEM = "6  F";
const char defFx_6[] PROGMEM = "7  7";
const char defFx_7[] PROGMEM = "8  8";
const char defFx_8[] PROGMEM = "9  9";
const char defFx_9[] PROGMEM = "10 10";
const char defFx_10[] PROGMEM = "11 11";
const char defFx_11[] PROGMEM = "12 12";
const char defFx_12[] PROGMEM = "13 13";
const char defFx_13[] PROGMEM = "14 14";
const char defFx_14[] PROGMEM = "15 15";
const char defFx_15[] PROGMEM = "16 16";
const char defFx_16[] PROGMEM = "17 17";
const char defFx_17[] PROGMEM = "18 18";
const char defFx_18[] PROGMEM = "19 19";
const char defFx_19[] PROGMEM = "20 20";
const char defFx_20[] PROGMEM = "21 21";
const char defFx_21[] PROGMEM = "22 22";
const char defFx_22[] PROGMEM = "23 23";
const char defFx_23[] PROGMEM = "24 24";*/

const char* const defFx[] PROGMEM = {
  defFx_0,defFx_1,defFx_2,defFx_3,defFx_4,
  defFx_5,defFx_6,defFx_7,defFx_8,defFx_9,
  defFx_10,defFx_11,defFx_12,defFx_13,defFx_14,
  defFx_15,defFx_16,defFx_17,defFx_18,defFx_19,
  defFx_20,defFx_21,defFx_22,defFx_23
};

const char layoutName_0[] PROGMEM = "Red";
const char layoutName_1[] PROGMEM = "Cyan";
const char layoutName_2[] PROGMEM = "Green";
const char layoutName_3[] PROGMEM = "Purple";
const char* const layoutName[] PROGMEM = {layoutName_0,layoutName_1,layoutName_2,layoutName_3};

/*const char menu0_0[] PROGMEM = "Settings";
const char menu0_1[] PROGMEM = "Swap Buttons";
const char menu0_2[] PROGMEM = "Save button states";
const char menu0_3[] PROGMEM = "Edit button labels";
const char menu0_4[] PROGMEM = "Set the boot patch";
const char menu0_5[] PROGMEM = "Calibrate exp pedal";
const char menu0_6[] PROGMEM = "Reset device";
const char* const menu0[] PROGMEM = {
  menu0_0,menu0_1,menu0_2,menu0_3,menu0_4,menu0_5,menu0_6
};*/


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
  bool boolArr[3][8];
  
  int first_location = eeprom_states_shift + (PATCH_STATE-1) * 3;
  
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
  bool boolArr[3][8];
  
  int first_location = eeprom_states_shift + (PATCH_STATE-1) * 3;
  
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
    EEPROM.write(first_location+i, binByte);
  }
}

void clear_eeprom() {
  bool a[8] = {0,0,0,0,0,0,0,0};
  byte emptyByte = boolArrayToByte(a);
  for(int i=0; i<1024; i++) {
    EEPROM.write(eeprom_states_shift+i, emptyByte);
  }
}

void write_default_fx() {
  
  char fxName[13];
  
  // Write
  for(uint8_t i=0; i<24; i++) {
    strcpy_P(fxName, (char*)pgm_read_word(&( defFx[i] ))); // get the couple short+long_name
    //fx[0]=i+1;  // consecutive cc numbers from 1 to 24

    int charLocation = eeprom_fx_shift + (i * 14);  // +0 first location
    EEPROM.write(charLocation, i + 60);  // write consecutive cc numbers from 60 to 84
    
    for(uint8_t j=1; j<14; j++) {
      
      int charLocation = eeprom_fx_shift + (i * 14) + j;
      
      EEPROM.write(charLocation, fxName[j-1]);
    }
    strcpy(fxName, "             ");
    
  }
  
}

uint8_t getFxChannel(uint8_t virtualButton) {
  byte channel = EEPROM.read(eeprom_fx_shift + (virtualButton*14));
  
  return channel;
}

void getFxShortName(uint8_t virtualButton, char * fx) { // button from 0 to 23
  //strcpy(fx, "   ");
  
  for(uint8_t i=1; i<4; i++) {
    fx[i-1] = EEPROM.read(eeprom_fx_shift + (virtualButton*14) + i);
  }
  fx[3]='\0';
}

void getFxLongName(uint8_t virtualButton, char fx[10]) { // button from 0 to 23
  //strcpy(fx, "          ");
  for(uint8_t i=4; i<14; i++) {
    fx[i-4] = EEPROM.read(eeprom_fx_shift + (virtualButton*14) + i);
  }
  fx[10]='\0';
}


void setFxChannel(uint8_t virtualButton, byte channel) {
  EEPROM.write(eeprom_fx_shift + (virtualButton*14), channel);
}

void writeFxShortName(uint8_t virtualButton, char fx[]) { // button from 0 to 23
  
  for(uint8_t i=1; i<4; i++) {
    EEPROM.write(eeprom_fx_shift + (virtualButton*14) + i, fx[i-1]);
  }
}

void setFxLongName(uint8_t virtualButton, char fx[10]) { // button from 0 to 23
  
  for(uint8_t i=4; i<14; i++) {
    EEPROM.write(eeprom_fx_shift + (virtualButton*14) + i, fx[i-4]);
  }
}




#endif /* DEVICE_SETTINGS_H */
