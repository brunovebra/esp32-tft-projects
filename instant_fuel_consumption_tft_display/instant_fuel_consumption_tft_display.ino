#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include "include/coords/coords.h"

/* PINOUT */
#define TFT_MISO   -1 // Not used
#define TFT_MOSI   19
#define TFT_SCLK   18
#define TFT_CS     5
#define TFT_DC     16
#define TFT_RST    23
#define TFT_BL     4  // Backlight

/* Display Orientation */
#define PORTRAIT          0
#define LANDSCAPE         1
#define PORTRAIT_FLIPPED  2
#define LANDSCAPE_FLIPPED 3
#define DISPLAY_ORIENTATION LANDSCAPE

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

/* Display size */
#define DISPLAY_WIDTH  135
#define DISPLAY_HEIGHT 240


/* Colors */
#define BG_COLOR        ST77XX_BLACK
#define PRIMARY_COLOR   ST77XX_WHITE
#define SECONDARY_COLOR ST77XX_WHITE
#define TEXT_COLOR      ST77XX_WHITE

int currentValue = -1;

class DynamicBar {
  public:
    SmartCoords container;
    SmartCoords barVar, cleaningBar, limitBar;
    SmartCoords txtFixed, txtVar;

    uint16_t barMaxValue = 15;

    const uint8_t txtSize = 2;
    const uint8_t txtHeight = 6; // px
    const uint16_t sep = 5; // px

    void init(uint16_t x, uint16_t y) {
      txtFixed.init(x,y);
      txtFixed.set_x0y0(0, 0);

      limitBar.init(x,y);
      limitBar.set_x0y0(180, 0);
      limitBar.set_x1y1(180,38);
      
      barVar.init(x,y);
      barVar.set_x0y0(0, 18);
      barVar.set_x1y1(0, 38);
      
      cleaningBar.init(x,y);
      cleaningBar.set_x0y0(180, 18);
      cleaningBar.set_x1y1(180, 38);

      txtVar.init(x,y);
      txtVar.set_x0y0(0, 0);
    }

    void drawFrame() {
      tft.setTextColor(TEXT_COLOR, BG_COLOR);
      tft.setCursor(txtFixed.abs.x0, txtFixed.abs.y0);
      tft.setTextSize(txtSize);
      tft.print("L/h:");

      tft.drawLine(
        limitBar.abs.x0, limitBar.abs.y0,
        limitBar.abs.x1, limitBar.abs.y1,
        SECONDARY_COLOR
      );
    }

    void drawBar(int16_t value) {
      static int16_t prev_value = 0;

      value = constrain(value, 0, barMaxValue);
      uint16_t fillWidth = map(value, 0, barMaxValue, 0, 180);

      // Calculate the area to clean/draw, it cleans just the remianing bar
      cleaningBar.set_x1(barVar.rel.x1);
      barVar.set_width(fillWidth);
      cleaningBar.set_x0(barVar.rel.x1);

      // Draw or clean the bar depending
      if(value > prev_value){
        tft.fillRect(
          barVar.abs.x0, barVar.abs.y0,
          barVar.rel.w,  barVar.rel.h,
          PRIMARY_COLOR
        );
      }else{
        tft.fillRect(
          cleaningBar.abs.x0, cleaningBar.abs.y0,
          cleaningBar.rel.w,  cleaningBar.rel.h,
          BG_COLOR
        );
      }
      //Serial.printf("barVar -      x0: %d, y0: %d | x1: %d, y1: %d\n", barVar.rel.x0, barVar.rel.y0, barVar.rel.x1, barVar.rel.y1);
      //Serial.printf("cleaningBar - x0: %d, y0: %d | x1: %d, y1: %d\n", cleaningBar.rel.x0, cleaningBar.rel.y0, cleaningBar.rel.x1, cleaningBar.rel.y1);
      
      prev_value = value;
    }
};

DynamicBar fuel_consuption;

void setup() {
  Serial.begin(115200);
  tft.init(DISPLAY_WIDTH, DISPLAY_HEIGHT);

  pinMode(TFT_BL, OUTPUT);
  analogWrite(TFT_BL, 255);  // Brillo máximo

  tft.setRotation(DISPLAY_ORIENTATION); // Landscape
  tft.fillScreen(BG_COLOR);
  
  fuel_consuption.init(50,60);
  fuel_consuption.drawFrame();
}

void loop() {
  if (Serial.available()) {
    int newValue = Serial.parseInt();
    Serial.println("ACK");
    if (newValue != currentValue) {
      fuel_consuption.drawBar(newValue);
      currentValue = newValue;
    }

    // Limpia buffer
    while (Serial.available()) Serial.read();
  }
}
