#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

/* PINOUT */
#define TFT_MISO   -1 // Not used
#define TFT_MOSI   19
#define TFT_SCLK   18
#define TFT_CS     5
#define TFT_DC     16
#define TFT_RST    23
#define TFT_BL     4  // Backlight

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

class Coords {
  public:
    uint16_t x0, y0, x1, y1, widht, height;
};

class DynamicBar {
  public:
    Coords obj;
    Coords barVar, cleaningBar, limitBar;
    Coords txtFixed, txtVar;

    uint16_t barMaxValue = 15;
    const uint16_t barMaxWidth = obj.x1 - obj.x0;

    const uint8_t txtSize = 2;
    const uint8_t txtHeight = 6; // px
    const uint16_t sep = 5; // px

    const uint16_t txtOffset = txtSize * txtHeight;
    const uint16_t txtEnd_y = obj.y0 + txtOffset;

    DynamicBar(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){
      obj.x0 = x0;
      obj.y0 = y0;
      obj.x1 = x1;
      obj.y1 = y1;
    }
    
    void drawFrame() {
      txtFixed.x0 = obj.x0;
      txtFixed.y0 = obj.y0;
      // txtFixed.x0 = ?; // TDB
      txtFixed.y1 = obj.y0 + txtHeight * txtSize;
      tft.setTextColor(TEXT_COLOR, BG_COLOR);
      tft.setCursor(txtFixed.x0, txtFixed.y0);
      tft.setTextSize(txtSize);
      tft.print("L/h:");

      limitBar.x0 = obj.x1,
      limitBar.y0 = txtFixed.y1 + sep,
      limitBar.x1 = obj.x1,
      limitBar.y1 = obj.y1;
      tft.drawLine(
        limitBar.x0, limitBar.y0,
        limitBar.x1, limitBar.y1,
        SECONDARY_COLOR
      );
    }

    void drawBar(uint16_t value) {
      value = constrain(value, 0, barMaxValue);
      uint16_t fillWidth = map(value, 0, barMaxValue, 0, barMaxWidth);

      // Limpiar área de la barra

      // Dibujar relleno
      uint16_t bar_x0 = obj.x0,
               bar_y0 = txtEnd_y + sep,
               bar_w = fillWidth,
               bar_h = obj.y1 - bar_y0 + 1;
      tft.fillRect(
        bar_x0, bar_y0,
        bar_w, bar_h,
        PRIMARY_COLOR
      );
    }
};

DynamicBar bar(20, 55, 200, 90);

void setup() {
  Serial.begin(115200);
  tft.init(DISPLAY_WIDTH, DISPLAY_HEIGHT);

  pinMode(TFT_BL, OUTPUT);

  analogWrite(TFT_BL, 255);  // Brillo máximo

  tft.setRotation(3); // Landscape
  tft.fillScreen(BG_COLOR);

  bar.drawFrame();
}

void loop() {
  if (Serial.available()) {
    int newValue = Serial.parseInt();

    if (newValue != currentValue) {
      bar.drawBar(newValue);
      currentValue = newValue;
    }

    // Limpia buffer
    while (Serial.available()) Serial.read();
  }
}
