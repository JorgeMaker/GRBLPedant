#include <Arduino.h>
#include "Definitions.h"

GRBLDrivrer::GRBLDrivrer(uint8_t controlPin) {
  controlPIN = controlPin;
  pinMode(controlPIN, OUTPUT);
  digitalWrite(controlPIN, HIGH);
  receivedChars = new char[numChars];

}
void GRBLDrivrer::sendCommand(String txCmd) {
  digitalWrite(controlPIN, HIGH);
  delay(50);
  Serial2.print(txCmd + '\r');
  delay(50);
  digitalWrite(controlPIN, LOW);
}

void GRBLDrivrer::sendCancel() {
  sendCommand(CMD_JOG_CANCEL);
}

void GRBLDrivrer::recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  while (Serial2.available() > 0 && newDataArrived == false) {
    rc = Serial2.read();
    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    } else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newDataArrived = true;
    }
  }
}
void GRBLDrivrer::performJoystickAction(JoystickMovement movement) {
  switch (movement) {
  case XPLUS:
    sendCommand("G91 G0 X1 F1800");
    recvWithEndMarker();
    newDataArrived = false;
    lastJoystickReadWasNOTZero = true;
    break;
  case XMINUS:
    sendCommand("G91 G0 X-1 F1800");
    recvWithEndMarker();
    newDataArrived = false;
    lastJoystickReadWasNOTZero = true;
    break;
  case YPLUS:
    sendCommand("G91 G0 Y1 F1800");
    recvWithEndMarker();
    newDataArrived = false;
    lastJoystickReadWasNOTZero = true;
    break;
  case YMINUS:
    sendCommand("G91 G0 Y-1 F1800");
    recvWithEndMarker();
    newDataArrived = false;
    lastJoystickReadWasNOTZero = true;
    break;
  case XPLUS_YPLUS:
    sendCommand("G91 G0 X1Y1 F1800");
    recvWithEndMarker();
    newDataArrived = false;
    lastJoystickReadWasNOTZero = true;
    break;
  case XPLUS_YMINUS:
    sendCommand("G91 G0 X1Y-1 F1800");
    recvWithEndMarker();
    newDataArrived = false;
    lastJoystickReadWasNOTZero = true;
    break;
  case XMINUS_YMINUS:
    sendCommand("G91 G0 X-1Y-1 F1800");
    recvWithEndMarker();
    newDataArrived = false;
    lastJoystickReadWasNOTZero = true;
    break;
  case XMINUS_YPLUS:
    sendCommand("G91 G0 X-1Y+1 F1800");
    recvWithEndMarker();
    newDataArrived = false;
    lastJoystickReadWasNOTZero = true;
    break;
  case NOT_MOVE:
    lastJoystickReadWasNOTZero = false;
    sendCommand(CMD_JOG_CANCEL);
    break;
  default:
    break;
  }
}

int GRBLDrivrer::sendCommandsFromRotaryEncoder(int movement, PedantStatus status) {
  if (movement == 1) {
    switch (status.getSelectedAxis()) {
    case X:
      sendCommand("$J=G21 G91 X1 F600");
      break;
    case Y:
      sendCommand("$J=G21 G91 Y1 F600");
      break;
    case Z:
      sendCommand("$J=G21G91 Z1 F600");
      break;
    default:
      break;
    }
    Serial.println("Avance del rotary emcoder");
    return 0;
  }
  if (movement == -1) {
    switch (status.getSelectedAxis()) {
    case X:
      sendCommand("$J=G21 G91 X-1 F600");
      break;
    case Y:
      sendCommand("$J=G21 G91 Y-1 F600");
      break;
    case Z:
      sendCommand("$J=G21G91 Z-1 F600");
      break;
    default:
      break;
    }
    Serial.println("Retoceso del rotary emcoder");
    return 0;
  }
}