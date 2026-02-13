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

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

const uint32_t bg_color = ST77XX_BLACK;

// Configuración de la barra
const int barX = 20;
const int barY = 60;
const int barWidth = 200;
const int barHeight = 30;
const int maxValue = 15;

int currentValue = -1;

void drawBar(int value) {
  // Limitar rango
  value = constrain(value, 0, maxValue);

  // Limpiar área de la barra
  tft.fillRect(barX, barY, barWidth, barHeight, bg_color);
  tft.drawRect(barX, barY, barWidth, barHeight, ST77XX_WHITE);

  // Calcular ancho proporcional
  int fillWidth = map(value, 0, maxValue, 0, barWidth);

  // Dibujar relleno
  tft.fillRect(barX, barY, fillWidth, barHeight, ST77XX_GREEN);

  // Mostrar valor numérico
  tft.setTextColor(ST77XX_WHITE, bg_color);
  tft.setCursor(barX, barY - 25);
  tft.setTextSize(2);
  tft.print("L/h: ");
  tft.print(value);
}

void setup() {
  Serial.begin(115200);

  SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);

  pinMode(TFT_BL, OUTPUT);
  analogWrite(TFT_BL, 255);  // Brillo máximo

  tft.init(135, 240);
  tft.setRotation(1);  // Landscape 
  tft.fillScreen(bg_color);

  drawBar(0);
}

void loop() {
  if (Serial.available()) {
    int newValue = Serial.parseInt();

    if (newValue != currentValue) {
      currentValue = newValue;
      drawBar(currentValue);
    }

    // Limpia buffer
    while (Serial.available()) Serial.read();
  }
}
