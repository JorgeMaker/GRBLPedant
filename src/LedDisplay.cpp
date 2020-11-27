#include "Definitions.h"

#include <Arduino.h>

/***************************
 *       Led Display  
 * *************************/

LedDisplay::LedDisplay(uint8_t xPIN, uint8_t yPIN, uint8_t zPIN) {

  ledXPIN = xPIN;
  ledYPIN = yPIN;
  ledZPIN = zPIN;

  pinMode(ledXPIN, OUTPUT);
  pinMode(ledYPIN, OUTPUT);
  pinMode(ledZPIN, OUTPUT);

}

void LedDisplay::showSelectedAxis(AxisSelection selection) {
  if (selection == X) {
    digitalWrite(ledXPIN, HIGH);
    digitalWrite(ledYPIN, LOW);
    digitalWrite(ledZPIN, LOW);
  } else if (selection == Y) {
    digitalWrite(ledXPIN, LOW);
    digitalWrite(ledYPIN, HIGH);
    digitalWrite(ledZPIN, LOW);
  } else if (selection == Z) {
    digitalWrite(ledXPIN, LOW);
    digitalWrite(ledYPIN, LOW);
    digitalWrite(ledZPIN, HIGH);
  }
}