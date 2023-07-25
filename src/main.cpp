/*
  ESP32_Compass - Programme to demonstrate functionality of HMC5883 triple
  axis magnetic sensor in conjunction with an LCD display.  Graphical code 
  adaptation from Volos Projects https://www.youtube.com/@VolosProjects
  for rotating display.
  https://www.youtube.com/c/jordanrubin6502
  2023 Jordan Rubin.
*/

#include <Arduino.h>
#include <TFT_eSPI.h> 
#include <QMC5883LCompass.h>
#include "fonts.h"

#define color1 TFT_WHITE
#define color2  0x8410
#define color3 TFT_ORANGE
#define color4 0x15B3
#define color5 0x00A3

QMC5883LCompass compass;
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite img = TFT_eSprite(&tft);

float lastValue=0;
double rad=0.01745;
float x[360];
float y[360];
float px[360];
float py[360];
float lx[360];
float ly[360];
int r=100;
int sx=110;
int sy=116;
int angle=0;
int lastAngle=0;
String lastHeading = "";
String cc[12]={"0","30","60","90","120","150","180","210","240","270","300","330"};
int start[12]; //was 10
int startP[60]; //was 60

void setup() {
  Serial.begin(115200);
  compass.init();
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(4);  
  tft.setTextColor(color4,TFT_BLACK);
  tft.drawString("COMPASS PROGRAMME",120,4);
  img.setSwapBytes(true);
  img.createSprite(220, 105);
  img.setTextDatum(4);
  int b,b2 =0;
  for(int i=0;i<360;i++){
    x[i]=(r*cos(rad*i))+sx;
    y[i]=(r*sin(rad*i))+sy;
    px[i]=((r-16)*cos(rad*i))+sx;
    py[i]=((r-16)*sin(rad*i))+sy;
    lx[i]=((r-24)*cos(rad*i))+sx;
    ly[i]=((r-24)*sin(rad*i))+sy;
    if(i%30==0){      //12th was 10th at %36
     start[b]=i;
     b++;
    }
    if(i%6==0){
     startP[b2]=i;
     b2++;
    }      
  }
}

void loop() {
  compass.read();
  int value = compass.getAzimuth();
  angle = value;
  String heading;
  if ( ((angle >= 338) && (angle <= 359)) || ((angle >=0 ) && (angle <=22 )) ){ heading = " N";}
  else if ( (angle >= 23) && (angle <= 67)){heading = "NE";}
  else if ( (angle >= 68) && (angle <= 113)){heading = " E";}
  else if ( (angle >= 114) && (angle <= 157)){heading = "SE";}
  else if ( (angle >= 158) && (angle <= 202)){heading = " S";}
  else if ( (angle >= 203) && (angle <= 248)){heading = "SW";}
  else if ( (angle >= 249) && (angle <= 292)){heading = " W";}
  else if ( (angle >= 293) && (angle <= 337)){heading = "NW";}
  if (heading != lastHeading){
    tft.setTextColor(TFT_BLACK,TFT_BLACK);
    tft.setFreeFont(&Slackey_Regular_16);
    tft.drawString(lastHeading,120,18); 
    tft.setTextColor(color3,TFT_BLACK);
    tft.setFreeFont(&Slackey_Regular_16);
    tft.drawString(heading,120,18);
    lastHeading = heading; 
  }
  Serial.print("A: ");  Serial.println(angle);
  if(angle!=lastAngle){
    lastAngle=angle;
    angle = (angle-270)*-1;
    if (angle <0){angle = angle + 360;}
    img.fillSprite(TFT_BLACK);
    img.fillCircle(sx,sy,124,color5);
    img.setTextColor(TFT_WHITE,color5);
    img.setFreeFont(&FreeSans9pt7b);
    for(int i=0;i<12;i++){
      if(start[i]+angle<360){
        img.drawString(cc[i],x[start[i]+angle],y[start[i]+angle]);
        img.drawLine(px[start[i]+angle],py[start[i]+angle],lx[start[i]+angle],ly[start[i]+angle],color1);
      }
      else {
        img.drawString(cc[i],x[(start[i]+angle)-360],y[(start[i]+angle)-360]);
        img.drawLine(px[(start[i]+angle)-360],py[(start[i]+angle)-360],lx[(start[i]+angle)-360],ly[(start[i]+angle)-360],color1);
      }
    }
    img.setFreeFont(&DSEG7_Modern_Bold_20);
    img.drawString(String(value),sx,sy-22);
    img.setTextFont(0);
    img.drawString("AZMUTH",sx,sy-42);
    for(int i=0;i<60;i++){
      if(startP[i]+angle<360){
        img.fillCircle(px[startP[i]+angle],py[startP[i]+angle],1,color2);
      }
      else{
        img.fillCircle(px[(startP[i]+angle)-360],py[(startP[i]+angle)-360],1,color2);
      }
    }
    img.fillTriangle(sx-1,sy-70,sx-5,sy-56,sx+4,sy-56,TFT_ORANGE);
    img.pushSprite(10, 30);
  }
  delay(250);
}