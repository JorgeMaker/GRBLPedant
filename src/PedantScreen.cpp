#include "Definitions.h"
#include <Arduino.h>

/***************************
 *   Pedant Screen Display  
 * *************************/
PedantScreenDisplay::PedantScreenDisplay(uint8_t h, uint8_t w, uint8_t r) {
  hieght = h;
  width = w;
  reset = r;
  display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, & Wire, OLED_RESET);
}
void PedantScreenDisplay::initLCDScreen(PedantStatus pedantStatus) {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
    ; // Don't proceed, loop forever
  }
  // Clear the buffer
  display.clearDisplay();
  display.display();
  display.fillRect(0, 0, 128, 16, WHITE);
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(5, 0);
  display.print("Llopis CNC");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5, 25);
  display.print("Axis:");
  display.setTextSize(2);
  display.setCursor(5, 45);
  display.print("Scale:");
  printScaleToScreen(pedantStatus.getXScaleString());
  display.display();
  printAxisToScreen("X");
}

void PedantScreenDisplay::printAxisToScreen(String axis) {
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(3);
  display.setCursor(90, 19);
  display.print(axis);
  display.display();
}
void PedantScreenDisplay::printScaleToScreen(String scaleString) {
  display.setTextSize(2);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(80, 45);
  display.print(scaleString);
  display.display();
}