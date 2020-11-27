#include "Definitions.h"
#include <Arduino.h>

/***************************
 *       Pedant Buttons   
 * *************************/

Button::Button(uint8_t buttonPin){
    pin = buttonPin;
}

void Button::begin(){
  pinMode(pin, INPUT_PULLUP);
  state = digitalRead(pin);
  buttonScaleLastChange = 0;
}
bool Button::isPushed(){
      unsigned long currentMillis = millis();
      if ((currentMillis - buttonScaleLastChange) < 100){
         return false;
      }
      else{
        bool v = digitalRead(pin);
        if (v != state) {
              state = v;
            if (!state) {
              return true;
            }
        }
      buttonScaleLastChange = currentMillis;
    return false;
  }
}

PedantButtons::PedantButtons (uint8_t xPIN, uint8_t yPIN, uint8_t zPIN, uint8_t scPIN, uint8_t stPIN){

    buttonX = new Button(xPIN);
    buttonY =  new Button(yPIN);
    buttonZ =  new Button(zPIN);
    buttonSCP =  new Button(scPIN);
    buttonSTP =  new Button(stPIN);

    buttonX->begin();
    buttonY->begin();
    buttonZ->begin();
    buttonSCP->begin();
    buttonSTP->begin();

    xPressedFlag = false;
    yPressedFlag = false;
    zPressedFlag = false;
    scalePressedFlag = false;
    stopPressedFlag = false;

}
PedantButtonsState PedantButtons::getPreviousState(){
    return previousStateButtons;
}
PedantButtonsState PedantButtons::readCurrentState(){

    currentStateButtons.buttonXisPressed = buttonX->isPushed();
    if (currentStateButtons.buttonXisPressed) xPressedFlag = true;

    currentStateButtons.buttonYisPressed = buttonY->isPushed();
    if (currentStateButtons.buttonYisPressed) yPressedFlag = true;

    currentStateButtons.buttonZisPressed = buttonZ->isPushed();
    if (currentStateButtons.buttonZisPressed) zPressedFlag = true;

    currentStateButtons.buttonSTisPressed = buttonSCP->isPushed();
    if (currentStateButtons.buttonSTisPressed) scalePressedFlag = true;
    
    currentStateButtons.buttonSCisPressed = buttonSTP->isPushed();
    if (currentStateButtons.buttonSCisPressed) stopPressedFlag = true;

    return currentStateButtons;
}

bool PedantButtons::hasXAxisBennPressed(){
  if (xPressedFlag) {
      xPressedFlag = false;
      return true;
  }
  return false;
}
bool PedantButtons::hasYAxisBennPressed(){
  if (yPressedFlag) {
      yPressedFlag = false;
      return true;
  }
  return false;
}
bool PedantButtons::hasZAxisBennPressed(){
  if (zPressedFlag) {
      zPressedFlag = false;
      return true;
  }
  return false;
}


bool PedantButtons::hasScaleBeenPressed(){
  if (scalePressedFlag) {
      scalePressedFlag = false;
      return true;
  }
  return false;
}

bool PedantButtons::hasStopBeenPressed(){
  if (stopPressedFlag) {
      stopPressedFlag = false;
      return true;
  }
  return false;
}

int PedantButtons::numOfPressedButons(){
  
  int numOfPressedButtons = 0;
  
  if (currentStateButtons.buttonXisPressed){
    numOfPressedButtons++;
  }
  if (currentStateButtons.buttonYisPressed){
    numOfPressedButtons++;
  }
  if (currentStateButtons.buttonZisPressed){
    numOfPressedButtons++;
  }
  if (currentStateButtons.buttonSCisPressed){
    numOfPressedButtons++;
  }
  if (currentStateButtons.buttonSTisPressed){
    numOfPressedButtons++;
  }
  return numOfPressedButtons;
}

AxisSelection PedantButtons::getAxisSelected(){
  
    AxisSelection selectedAxis = X;
    int pressedButons = numOfPressedButons();
    if (pressedButons == 0){
      selectedAxis = UKNOWN;
    }
    else if (pressedButons == 1){
      
      if (currentStateButtons.buttonXisPressed == true){
        selectedAxis = X;
      }
      if (currentStateButtons.buttonYisPressed  == true){
        selectedAxis = Y;
      }
      if (currentStateButtons.buttonZisPressed  == true){
        selectedAxis = Z;
      }
      if (currentStateButtons.buttonSCisPressed  == true){
        selectedAxis = UKNOWN;
      }
      if (currentStateButtons.buttonSTisPressed  == true){
        selectedAxis = UKNOWN;
      }
    }
    return selectedAxis;
}