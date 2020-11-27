#include "Definitions.h"

#include <Arduino.h>

/***************************
 *       Joystick  
 * *************************/

#define MID_X 1998
#define MID_Y 1827

Joystick::Joystick(uint8_t xPin, uint8_t yPin, int thresholdX, int thresholdY) {

  thr_x = thresholdX;
  thr_y = thresholdY;

  xPIN = xPin;
  yPIN = yPin;
  pinMode(xPin, OUTPUT);
  pinMode(yPin, OUTPUT);

}

JoystickMovement Joystick::readMovement() {

  int xValue = analogRead(xPIN);
  int yValue = analogRead(yPIN);

  if (xValue < (MID_X - thr_x)) {
    if (yValue < (MID_Y - thr_y)) {
      return XMINUS_YMINUS;
    } else if (yValue > (MID_Y + thr_y)) {
      return XPLUS_YMINUS;
    } else {
      return YMINUS;
    }
  } else if (xValue > (MID_X + thr_x)) {
    if (yValue < (MID_Y - thr_y)) {
      return XMINUS_YPLUS;

    } else if (yValue > (MID_Y + thr_y)) {
      return XPLUS_YPLUS;
    } else {
      return YPLUS;
    }
  } else {
    if (yValue < (MID_Y - thr_y)) {
      return XMINUS;

    } else if (yValue > (MID_Y + thr_y)) {
      return XPLUS;
    } else {
      return NOT_MOVE;
    }
  }
}

String Joystick::movementToSring(JoystickMovement movement) {

  switch (movement) {
  case XPLUS: // 0,1 mm
    return "X+";
    break;
  case XMINUS: // 0,5 mm
    return "X-";
    break;
  case YPLUS: //  1 mm
    return "Y+";
    break;
  case YMINUS: //  10mm
    return "Y-";
    break;
  case XPLUS_YPLUS: //  50 mm
    return "X+Y+";
    break;
  case XPLUS_YMINUS: //  50 mm
    return "X+Y-";
    break;
  case XMINUS_YMINUS: //  50 mm
    return "X-Y-";
    break;
  case XMINUS_YPLUS: //  50 mm
    return "X-Y+";
    break;
  case NOT_MOVE: //  50 mm
    return "NOT_MOVE";
    break;
  }
}

String Joystick::readStringMovement() {
  return movementToSring(readMovement());
}