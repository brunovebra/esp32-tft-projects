#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <ELMo.h>
#include "include/coords/coords.h"

/* ST7789V PINOUT */
#define TFT_MISO   -1 // Not used
#define TFT_MOSI   19
#define TFT_SCLK   18
#define TFT_CS     5
#define TFT_DC     16
#define TFT_RST    23
#define TFT_BL     4  // Backlight

/* Display size */
#define DISPLAY_WIDTH  135
#define DISPLAY_HEIGHT 240

/* Display Orientation */
#define PORTRAIT            0
#define LANDSCAPE           1
#define PORTRAIT_FLIPPED    2
#define LANDSCAPE_FLIPPED   3
#define DISPLAY_ORIENTATION LANDSCAPE

/* Colors */
#define BG_COLOR        ST77XX_BLACK
#define PRIMARY_COLOR   ST77XX_WHITE
#define SECONDARY_COLOR ST77XX_WHITE
#define TEXT_COLOR      ST77XX_WHITE

/* Containers position */
#define FUEL_COMNSUMPTION_X0 30
#define FUEL_COMNSUMPTION_Y0 30
#define FUEL_COMNSUMPTION_X1 180
#define FUEL_COMNSUMPTION_Y1 30
#define FUEL_COMNSUMPTION_MAX_BAR_VALUE 15.0

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

class DynamicBar {
  private:
    /* Parameters default values */
    float barMaxValue = 15.0; // units
    uint16_t barLenght = 180; // px
    uint16_t txtSize = 2;

    const uint16_t txtHeight = 6; // px
    const uint16_t sep = 5; // px

  public:
    SmartCoords container;
    SmartCoords barVar, cleaningBar, limitBar;
    SmartCoords txtFixed, txtVar;

    void init(uint16_t _x0, uint16_t _y0, uint16_t _x1, uint16_t _y1, uint16_t _barMaxValue) {
      container.init(container.x0, container.y0);
      container.set_x0y0(_x0, _y0);
      container.set_x1y1(_x1, _y1);

      barMaxValue = _barMaxValue;

      txtFixed.init(container.x0, container.y0);
      txtFixed.set_x0y0(0, 0);

      uint16_t separation_for_bar = txtHeight * txtSize + sep;

      limitBar.init(container.x0, container.y0);
      limitBar.set_x0y0(container.x1, 0);
      limitBar.set_x1y1(container.x1, container.y1 - 1);

      barVar.init(container.x0,container.y0);
      barVar.set_x0y0(0, separation_for_bar);
      barVar.set_x1y1(0, container.y1);

      cleaningBar.init(container.x0,container.y0);
      cleaningBar.set_x0y0(container.x1, separation_for_bar);
      cleaningBar.set_x1y1(container.x1, container.y1);

      txtVar.init(container.x0,container.y0);
      txtVar.set_x0y0(0, 0);
    }

    void set_parameters(uint16_t _barMaxValue, uint16_t _barLenght) {
      barMaxValue = _barMaxValue;
      barLenght = _barLenght;
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

    void drawBar(float value) {
      value = constrain(value, 0, barMaxValue); // Limit `value` up to `barMaxValue`
      static int16_t prev_width = 0;
      uint16_t width = (uint16_t) limitBar.x1 / barMaxValue * value; // Convertion from input value to width

      // Calculate the area to clean/draw, it cleans just the remianing bar
      uint16_t barVar_rel_x1_temp = barVar.x1;
      cleaningBar.set_x1(barVar.x1);
      barVar.set_width(width);
      cleaningBar.set_x0(barVar.x1);
      barVar.set_x0(barVar_rel_x1_temp);

      // Draw or clean the bar depending
      if(width > prev_width){
        tft.fillRect(
          barVar.abs.x0, barVar.abs.y0,
          barVar.w,  barVar.h,
          PRIMARY_COLOR
        );
      }else{
        tft.fillRect(
          cleaningBar.abs.x0, cleaningBar.abs.y0,
          cleaningBar.w,  cleaningBar.h,
          BG_COLOR
        );
      }

      prev_width = width;
      barVar.set_x0(0);
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

  fuel_consuption.init(FUEL_COMNSUMPTION_X0 ,FUEL_COMNSUMPTION_Y0, 
                       FUEL_COMNSUMPTION_X1 ,FUEL_COMNSUMPTION_Y1, 
                       FUEL_COMNSUMPTION_MAX_BAR_VALUE);
  fuel_consuption.drawFrame();
}

void loop() {
  static float currentValue = -1.0;
  if (Serial.available()) {
    float newValue = Serial.parseFloat();

    if (newValue != currentValue) {
      fuel_consuption.drawBar(newValue);
      currentValue = newValue;
    }

    // Limpia buffer
    while (Serial.available()) Serial.read();
  }
}
