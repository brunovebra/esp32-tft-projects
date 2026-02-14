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

class DynamicBar {
  public:
    uint16_t x0, y0, x1, y1;

    uint16_t barMaxValue = 15;
    const uint16_t barMaxWidth = x1 - x0;

    const uint8_t txtSize = 2;
    const uint8_t txtHeight = 6; // px
    const uint8_t v_sep = 5; // px

    const uint16_t txtOffset = txtSize * txtHeight;
    const uint16_t txtEnd_y = y0 + txtOffset;

    DynamicBar(uint16_t _x0, uint16_t _y0, uint16_t _x1, uint16_t _y1)
        : x0(_x0), y0(_y0), x1(_x1), y1(_y1) {
    }
    
    void drawFrame() {
      tft.setTextColor(TEXT_COLOR, BG_COLOR);
      tft.setCursor(x0, y0);
      tft.setTextSize(txtSize);
      tft.print("L/h:");

      uint16_t barLimit_x0 = x1,
               barLimit_y0 = txtEnd_y + v_sep,
               barLimit_x1 = x1,
               barLimit_y1 = y1;
      tft.drawLine(
        barLimit_x0, barLimit_y0,
        barLimit_x1, barLimit_y1,
        SECONDARY_COLOR
      );
    }

    void drawBar(uint16_t value) {
      value = constrain(value, 0, barMaxValue);
      uint16_t fillWidth = map(value, 0, barMaxValue, 0, barMaxWidth);

      // Limpiar área de la barra

      // Dibujar relleno
      uint16_t bar_x0 = x0,
               bar_y0 = txtEnd_y + v_sep,
               bar_w = fillWidth,
               bar_h = y1 - bar_y0 + 1;
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
