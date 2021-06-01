#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <U8g2lib.h>  // Display graphics
#include <SPI.h>      // Display "driver"

U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /* CS=*/ 10, /* reset=*/ 8);

struct screenLayout {
  char title[40];
  char fxLabel1[6];
  char fxLabel2[6];
  char fxLabel3[6];
  char fxLabel4[6];
  char fxLabel5[6];
  char fxLabel6[6];
  char fxDescr1[16];
  char fxDescr2[16];
  char fxDescr3[16];
  char fxDescr4[16];
  char fxDescr5[16];
  char fxDescr6[16];
  bool buttonState[6];
} scr;


void splash(){
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_sirclivethebold_tr);
    u8g2.drawStr(9,20,"midi");
    u8g2.setFont(u8g2_font_tenfatguys_tf);
    u8g2.drawStr(10,31,"CONTROLLER");
    u8g2.drawStr(60,43,"MAN");
    u8g2.setFont(u8g2_font_sirclive_tr);
    u8g2.drawStr(30,56,"Stomp 6");
  } while ( u8g2.nextPage() );
}



void buildScreen(){
  uint8_t xPos;
  uint8_t strLen;
  
  u8g2.firstPage();
  do {
    u8g2.setFontDirection(0);

    // Title
    //u8g2.setFont(u8g2_font_tenfatguys_t_all); // fat font
    //strLen = u8g2.getStrWidth(scr.title);

    //if(strLen>128) {
       // thin font
       u8g2.setFont(u8g2_font_tenthinnerguys_t_all);
       strLen = u8g2.getStrWidth(scr.title);
    //}
    if(strLen>128) {
       // thin font
       u8g2.setFont(u8g2_font_lastapprenticethin_tr);
       strLen = u8g2.getStrWidth(scr.title);
    }
    
    if(strLen>u8g2.getDisplayWidth())
      xPos=0; // align left
    else
      xPos=(u8g2.getDisplayWidth()/2)-(strLen/2);  // center screen
    u8g2.drawStr(xPos, 30+(u8g2.getMaxCharHeight()/2), scr.title);
  
    // Short Labels
    u8g2.setFont(u8g2_font_tenfatguys_tr);
  
    u8g2.setFontMode(1);  /* activate transparent font mode */
    u8g2.setDrawColor(2);
    u8g2.drawStr(1, 11, scr.fxLabel4);
    if(scr.buttonState[3])
      u8g2.drawBox(0, 0, 41, 18);
    //u8g2.setFontMode(0);
    
    strLen = u8g2.getStrWidth(scr.fxLabel5);
    xPos = 64-(strLen/2); // center string
    u8g2.drawStr(xPos, 11, scr.fxLabel5);
    if(scr.buttonState[4])
      u8g2.drawBox(43, 0, 41, 18);
    //u8g2.setFontMode(0);
    
    strLen = u8g2.getStrWidth(scr.fxLabel6);
    xPos = u8g2.getDisplayWidth()-strLen-1; // align right
    u8g2.drawStr(xPos, 11, scr.fxLabel6);
    if(scr.buttonState[5])
      u8g2.drawBox(87, 0, 41, 18);
    //u8g2.setFontMode(0);
  
    strLen = u8g2.getStrWidth(scr.fxLabel1);
    u8g2.drawStr(1, 63, scr.fxLabel1);
    if(scr.buttonState[0])
      u8g2.drawBox(0, 46, 41, 18);
  
    strLen = u8g2.getStrWidth(scr.fxLabel2);
    xPos = 64-(strLen/2); // center string
    u8g2.drawStr(xPos, 63, scr.fxLabel2);
    if(scr.buttonState[1])
      u8g2.drawBox(43, 46, 41, 18);
  
    strLen = u8g2.getStrWidth(scr.fxLabel3);
    xPos = u8g2.getDisplayWidth()-strLen-1; // align right
    u8g2.drawStr(xPos, 63, scr.fxLabel3);
    if(scr.buttonState[2])
      u8g2.drawBox(87, 46, 41, 18);
  
    // Descriptions
    u8g2.setFont(u8g2_font_trixel_square_tf);
    
    u8g2.drawStr(1, 17, scr.fxDescr4);
    
    strLen = u8g2.getStrWidth(scr.fxDescr5);
    xPos = 64-(strLen/2); // center string
    u8g2.drawStr(xPos, 17, scr.fxDescr5);
    
    strLen = u8g2.getStrWidth(scr.fxDescr6);
    xPos = u8g2.getDisplayWidth()-strLen-1; // right align
    u8g2.drawStr(xPos, 17, scr.fxDescr6);
    
    u8g2.drawStr(1, 52, scr.fxDescr1);
    
    strLen = u8g2.getStrWidth(scr.fxDescr2);
    xPos = 64-(strLen/2); // center string
    u8g2.drawStr(xPos, 52, scr.fxDescr2);
    
    strLen = u8g2.getStrWidth(scr.fxDescr3);
    xPos = u8g2.getDisplayWidth()-strLen-1;
    u8g2.drawStr(xPos, 52, scr.fxDescr3);
    
    //u8g2.drawStr(70,16,"--Patch Up--");
    //u8g2.drawStr(66,52,"--Patch Down--");
    //u8g2.setFontDirection(1);
    //u8g2.drawStr(0,20,"L. VIEW");
    //u8g2.setFontDirection(3);
    //u8g2.drawStr(128,44,"R. VIEW");
    //u8g2.drawFrame(6, 18, 117, 29);
  } while ( u8g2.nextPage() );
}

void init_display() {
  u8g2.begin();
}

#endif /* DISPLAY_H */
