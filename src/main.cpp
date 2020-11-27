#include <Arduino.h>
#include "Definitions.h"
#include "Config.h"

PedantStatus pedantStatus(X, xy0_1MM, xy0_1MM, z0_1MM);
PedantButtons buttons(X_BUTTON_PIN, Y_BUTTON_PIN, Z_BUTTON_PIN, SCALE_BUTTON_PIN, STOP_BUTTON_PIN);
Joystick joystick(JOYSTICK_VRX_PIN, JOYSTICK_VRY_PIN, THRESHOLD_X, THRESHOLD_Y);
LedDisplay ledDisplay(X_LED_PIN, Y_LED_PIN, Z_LED_PIN);
PedantScreenDisplay screen(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_RESET);
GRBLDrivrer grblDrivrer(13);
PedantButtonsState buttonsState;

/***************************
 *      Rotary Encoder  
 * *************************/

unsigned long rotaryMoveLastChange = 0;
int rotaryMovement = 0;

void attendRotaryEncoder();
void initEncoder(uint8_t s1Pin, uint8_t s2Pin);

void initEncoder(uint8_t s1Pin, uint8_t s2Pin) {
  pinMode(s1Pin, INPUT);
  pinMode(s2Pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(s1Pin), attendRotaryEncoder, RISING);
}

void attendRotaryEncoder() {
  unsigned long currentMillis = millis();
  if ((currentMillis - rotaryMoveLastChange) < 100) {
    return;
  } else {
    rotaryMoveLastChange = currentMillis;
    if (digitalRead(ROTARY_MOV_S1) && !digitalRead(ROTARY_MOV_S2)) {
      rotaryMovement = 1;
    }
    if (digitalRead(ROTARY_MOV_S1) && digitalRead(ROTARY_MOV_S2)) {
      rotaryMovement = -1;
    }
  }
}


void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  pinMode(CONTROL_SERIAL_PIN, OUTPUT);
  digitalWrite(CONTROL_SERIAL_PIN, LOW);
  initEncoder(ROTARY_MOV_S1, ROTARY_MOV_S2);
  screen.initLCDScreen(pedantStatus);
}
void loop() {
  buttonsState = buttons.readCurrentState();
  if (buttonsState.buttonSTisPressed) {
    grblDrivrer.sendCancel();
  };
  pedantStatus.updateSelectedAxis(buttonsState);
  screen.printAxisToScreen(pedantStatus.getSelectedAxisString());
  screen.printScaleToScreen(pedantStatus.getSelectedStringScale());
  ledDisplay.showSelectedAxis(pedantStatus.getSelectedAxis());
  grblDrivrer.performJoystickAction(joystick.readMovement());
  rotaryMovement = grblDrivrer.sendCommandsFromRotaryEncoder(rotaryMovement, pedantStatus);
}
