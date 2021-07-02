#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <U8g2lib.h>  // Display graphics
#include <SPI.h>      // Display "driver"

U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /* CS=*/ 10, /* reset=*/ 8);

struct screenLayout {
  char title[20];
  uint8_t ccNum;
  char fxLabel[6][4];
  char fxDescr[6][11];
  bool buttonState[6];
} scr;


/*void splash() {
  u8g2.firstPage();
  do {
    
    u8g2.setFont(u8g2_font_trixel_square_tf);
    u8g2.drawStr(10,19,"midi");
    u8g2.setFont(u8g2_font_tenfatguys_t_all);
    u8g2.drawStr(10,31,"CONTROLLER");
    u8g2.drawStr(60,43,"MAN");
    u8g2.setFont(u8g2_font_tenthinnerguys_t_all);
    u8g2.drawStr(45,58,"Stomp 6");
  } while ( u8g2.nextPage() );
}*/


void drawLayout(screenLayout scr){
  uint8_t xPos;
  uint8_t strLen;
  
  u8g2.firstPage();
  do {
    u8g2.setFontDirection(0);

    // Title
    u8g2.setFont(u8g2_font_tenfatguys_t_all); // fat font
    strLen = u8g2.getStrWidth(scr.title);

    if(strLen>126) {
       // thin font
       u8g2.setFont(u8g2_font_tenthinnerguys_t_all);
       strLen = u8g2.getStrWidth(scr.title);
    }
    
    xPos=(u8g2.getDisplayWidth()/2)-(strLen/2);  // center screen
    u8g2.drawStr(xPos, 30+(u8g2.getMaxCharHeight()/2), scr.title);

    // Short Labels
    u8g2.setFont(u8g2_font_tenfatguys_t_all);
  
    u8g2.setFontMode(1); //* activate transparent font mode 
    u8g2.setDrawColor(2);
    u8g2.drawStr(1, 11, scr.fxLabel[3]);
    if(scr.buttonState[3])
      u8g2.drawBox(0, 0, 41, 18);
    //u8g2.setFontMode(0);
    
    strLen = u8g2.getStrWidth(scr.fxLabel[4]);
    xPos = 64-(strLen/2); // center string
    u8g2.drawStr(xPos, 11, scr.fxLabel[4]);
    if(scr.buttonState[4])
      u8g2.drawBox(43, 0, 42, 18);
    //u8g2.setFontMode(0);
    
    strLen = u8g2.getStrWidth(scr.fxLabel[5]);
    xPos = u8g2.getDisplayWidth()-strLen-1; // align right
    u8g2.drawStr(xPos, 11, scr.fxLabel[5]);
    if(scr.buttonState[5])
      u8g2.drawBox(87, 0, 41, 18);
    //u8g2.setFontMode(0);
  
    strLen = u8g2.getStrWidth(scr.fxLabel[0]);
    u8g2.drawStr(1, 63, scr.fxLabel[0]);
    if(scr.buttonState[0])
      u8g2.drawBox(0, 46, 41, 18);
  
    strLen = u8g2.getStrWidth(scr.fxLabel[1]);
    xPos = 64-(strLen/2); // center string
    u8g2.drawStr(xPos, 63, scr.fxLabel[1]);
    if(scr.buttonState[1])
      u8g2.drawBox(43, 46, 42, 18);
  
    strLen = u8g2.getStrWidth(scr.fxLabel[2]);
    xPos = u8g2.getDisplayWidth()-strLen-1; // align right
    u8g2.drawStr(xPos, 63, scr.fxLabel[2]);
    if(scr.buttonState[2])
      u8g2.drawBox(87, 46, 41, 18);
  
    // Descriptions
    u8g2.setFont(u8g2_font_trixel_square_tf);
    
    u8g2.drawStr(1, 17, scr.fxDescr[3]);
    
    strLen = u8g2.getStrWidth(scr.fxDescr[4]);
    xPos = 64-(strLen/2); // center string
    u8g2.drawStr(xPos, 17, scr.fxDescr[4]);
    
    strLen = u8g2.getStrWidth(scr.fxDescr[5]);
    xPos = u8g2.getDisplayWidth()-strLen-1; // right align
    u8g2.drawStr(xPos, 17, scr.fxDescr[5]);
    
    u8g2.drawStr(1, 52, scr.fxDescr[0]);
    
    strLen = u8g2.getStrWidth(scr.fxDescr[1]);
    xPos = 64-(strLen/2); // center string
    u8g2.drawStr(xPos, 52, scr.fxDescr[1]);
    
    strLen = u8g2.getStrWidth(scr.fxDescr[2]);
    xPos = u8g2.getDisplayWidth()-strLen-1;
    u8g2.drawStr(xPos, 52, scr.fxDescr[2]);

    //u8g2.drawStr(70,16,"--Patch Up--");
    //u8g2.drawStr(66,52,"--Patch Down--");
    //u8g2.setFontDirection(1);
    //u8g2.drawStr(0,20,"L. LAYOUT");
    //u8g2.setFontDirection(3);
    //u8g2.drawStr(128,44,"R. LAYOUT");
    //u8g2.drawFrame(0, 18, 128, 28);
    u8g2.drawLine(0, 18, 128, 18);
    u8g2.drawLine(0, 45, 128, 45);

    if(scr.ccNum>0) {
      char ccStr[5]; // "#127"
      sprintf(ccStr, "#%d", scr.ccNum);
      strLen = u8g2.getStrWidth(ccStr);
      xPos=u8g2.getDisplayWidth()-strLen-1; // align right
      u8g2.drawStr(xPos, 43, ccStr);
    }
    
  } while ( u8g2.nextPage() );
}

void menu_nav_buttons(bool buttState[]) {
  
    // Navigation indicators
    u8g2.setFont(u8g2_font_twelvedings_t_all );
    /*u8g2.drawStr(0, 12, "{}<>,.[]");
    u8g2.drawStr(0, 28, "PQopOru");
    u8g2.drawStr(0, 46, "Tjksq");
    u8g2.drawStr(0, 62, "/A@U");*/

    if(buttState[3])
      u8g2.drawStr(1, 9, ",");   // left
    if(buttState[4])
      u8g2.drawStr(58, 8, "{");  // up
    if(buttState[5])
      u8g2.drawStr(117, 9, "."); // right
    if(buttState[0])
      u8g2.drawStr(0, 64, "/");   // confim
    if(buttState[1])
      u8g2.drawStr(58, 65, "}");  // down
    if(buttState[2])
      u8g2.drawStr(118, 64, "U"); // back
}

void drawButtonEdit(uint8_t selectedIndex, char fx[17]) {
  char title[20];
  uint8_t xPos = 0;
  uint8_t yPos = 30;

  bool buttState[6]={1,1,1,1,1,1};
  if(selectedIndex==0) {
    buttState[3] = 0;
  } else if(selectedIndex == 15) {
    buttState[5] = 0;
  }
  
  u8g2.firstPage();
  do {
    u8g2.setFontDirection(0);
    
    // Navigation indicators
    menu_nav_buttons(buttState);

    if(selectedIndex < 1) {
      strcpy(title, "Long Press Link");
      //u8g2.drawFrame(0, 32, 26, 10);
    } else if(selectedIndex < 2) {
      strcpy(title, "Double Click Link");
      //u8g2.drawFrame(0, 32, 26, 10);
    } else if(selectedIndex < 3) {
      strcpy(title, "Control Change N.");
      //u8g2.drawFrame(0, 32, 26, 10);
    } else if(selectedIndex < 6) {
      strcpy(title, "Short Name");
      u8g2.drawFrame(21, 34, 23, 10);
    } else {
      strcpy(title, "Long Name");
      u8g2.drawFrame(46, 34, 72, 10);
    }

    u8g2.setFont(u8g2_font_trixel_square_tf);
    u8g2.drawStr(1, 18, title);
    u8g2.setFont(u8g2_font_chroma48medium8_8r);
    
    for(uint8_t i=0; i<16; i++) {
      yPos=30;
      xPos = 1 + i*7;  // space + i * char width
      if(i>0) {
        xPos+=40; // space after long press
      }
      if(i>1) {
        xPos+=40; // space after doubleclick
      }
      if(i>2) {
        xPos-=80; // space after controlChange
        yPos=42;
      }
      if(i>5) {
        xPos+=4; // space after shortName (+cc)
      }

      char c[8];
      if(i==0 || i==1) {  // is button link
        if(fx[i] < 0) {
          fx[i]=24;  // 0 is not a button, means that button link is disabled
        } else if(fx[i] > 24) {
          fx[i]=0;
        }

        if(fx[i]==0) {
          sprintf(c, "%s", "Off");
        } else {
          uint8_t layout_n = (fx[i]-1) / 6;
          uint8_t button = fx[i] - (layout_n*6);
          char lsName[4];
          strcpy_P(lsName, (char*)pgm_read_word(&( shortLayoutName[layout_n] )));
          sprintf(c, "%s-%d", lsName, button);
        }
      } else if(i==2) {  // is ccnum
        if(fx[i] < 1) {
          fx[i]=126;
        } else if(fx[i] > 126) {
          fx[i]=1;
        }
        sprintf(c, "%d", fx[i]);
      } else {  // is string

        if(fx[i] == '\0') {
          fx[i] = 32;
        } else if(fx[i] < 31) {
          fx[i]=32;
        } else if(fx[i] < 32) {
          fx[i]=126;
        } else if(fx[i] > 126) {
          fx[i]=32;
        }
        c[0] = fx[i];
        c[1] = '\0';
      }
      
      u8g2.drawStr(xPos, yPos, c);
      
      if(i==selectedIndex) { // selected
        u8g2.drawBox(xPos, yPos-7, u8g2.getStrWidth(c)-1, 8);
      }
    }
    
  } while ( u8g2.nextPage() );
}

void drawListChooser(uint8_t property, uint8_t menu_id, uint8_t sel, uint8_t firstItemShift) {

  uint8_t m_option_len = getPMStrSize(menu_id);
  bool buttState[6]={1,1,1,0,1,0};
  uint8_t xPos;
  uint8_t yPos;
  uint8_t titleLen;
  uint8_t strLen;
  char title[20];
  getProperty(property, title);
  
  uint8_t menu_entries=7;
  if(m_option_len<7) {
    menu_entries = m_option_len;
  }
  
  u8g2.firstPage();
  do {
    u8g2.setFontDirection(0);
    
    menu_nav_buttons(buttState);

    // Menu
    u8g2.setFont(u8g2_font_trixel_square_tf);
    uint8_t char_height=7;//u8g2.getMaxCharHeight();
    u8g2.drawStr(1, 25, title);
    titleLen = u8g2.getStrWidth(title)+4;
    
    for(uint8_t i=firstItemShift; i<firstItemShift+menu_entries; i++) {

      char myString[20];
      getPMArrayVal(menu_id, i, myString);
      
      yPos = 14+(char_height*(i-firstItemShift));
      strLen = u8g2.getStrWidth(myString);
      u8g2.drawStr(titleLen+2, yPos-1, myString);
      
      if(i+1==sel) { // selected
        u8g2.drawBox(titleLen+1, yPos-char_height, strLen+4, 7);
      }
    }
    u8g2.drawLine(titleLen, char_height, titleLen, yPos-1);
    
  } while ( u8g2.nextPage() );
  
}

void drawConfirm(uint8_t property) {

  char title[20];
  getProperty(property, title);
  bool buttState[6]={1,0,1,0,0,0};
  
  u8g2.firstPage();
  do {
    u8g2.setFontDirection(0);
    
    menu_nav_buttons(buttState);

    u8g2.setFont(u8g2_font_tenthinnerguys_t_all);
    uint8_t xPos=(u8g2.getDisplayWidth()/2)-(u8g2.getStrWidth(title)/2); // center x
    uint8_t yPos = (u8g2.getDisplayHeight()/2)+(u8g2.getMaxCharHeight()/2); // center y
    u8g2.drawStr(xPos, yPos, title);
    
  } while ( u8g2.nextPage() );
  
}

void init_display() {
  u8g2.begin();
}

#endif /* DISPLAY_H */