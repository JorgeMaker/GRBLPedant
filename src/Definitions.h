#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <Wire.h>
#include <Config.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

enum AxisSelection {
  X,
  Y,
  Z,
  UKNOWN
};
enum zScale {
  z0_1MM,
  z0_5MM,
  z1MM,
  z10MM,
  z50MM
};
enum xyScale {
  xy0_1MM,
  xy0_5MM,
  xy1MM,
  xy10MM,
  xy50MM,
  xy100MM
};

/***************************
 *       Pedant Buttons 
 * *************************/

class Button {
  public:
    Button(uint8_t);
    void begin();
    bool isPushed();
  private:
    bool state;
    uint8_t pin;
    unsigned long buttonScaleLastChange;
};

typedef struct {
  bool buttonXisPressed;
  bool buttonYisPressed;
  bool buttonZisPressed;
  bool buttonSCisPressed;
  bool buttonSTisPressed;

}
PedantButtonsState;

class PedantButtons {

  public:
    PedantButtons(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
    PedantButtonsState getPreviousState();
    PedantButtonsState readCurrentState();
    int numOfPressedButons();
    bool hasXAxisBennPressed();
    bool hasYAxisBennPressed();
    bool hasZAxisBennPressed();
    bool hasScaleBeenPressed();
    bool hasStopBeenPressed();
    AxisSelection getAxisSelected();

  private:
    Button * buttonX;
    Button * buttonY;
    Button * buttonZ;
    Button * buttonSCP;
    Button * buttonSTP;
    bool scalePressedFlag;
    bool stopPressedFlag;
    bool xPressedFlag;
    bool yPressedFlag;
    bool zPressedFlag;
    PedantButtonsState currentStateButtons;
    PedantButtonsState previousStateButtons;

};

/***************************
 *       Pedant Status 
 * *************************/

class PedantStatus {

  public:
    PedantStatus(AxisSelection, xyScale, xyScale, zScale);
    void setSelectedAxis(AxisSelection);
    void modifyXScale(int);
    void modifyYScale(int);
    void modifyZScale(int);
    AxisSelection getSelectedAxis();
    String getSelectedAxisString();
    String getXScaleString();
    String getYScaleString();
    String getZScaleString();
    String getUknownScaleString();
    void updateSelectedAxis(PedantButtonsState);
    const static int INCREASE_OPP = 1;
    const static int DECREASE_OPP = -1;
    String getSelectedStringScale();

  private:
    AxisSelection selectedAxis;
    String xyScaleToString(xyScale);
    String zScaleToString(zScale);
    xyScale nextXYScaleValue(xyScale);
    xyScale previousXYScaleValue(xyScale);
    zScale nextZScaleValue(zScale);
    zScale previousZScaleValue(zScale);
    xyScale scaleX;
    xyScale scaleY;
    zScale scaleZ;
};
/***************************
 *       Joystick  
 * *************************/

enum JoystickMovement {
  XPLUS,
  XMINUS,
  YPLUS,
  YMINUS,
  XPLUS_YPLUS,
  XPLUS_YMINUS,
  XMINUS_YMINUS,
  XMINUS_YPLUS,
  NOT_MOVE
};

/***************************
 *       Joystick  
 * *************************/

class Joystick {
  public:
    Joystick(uint8_t, uint8_t, int, int);
    JoystickMovement readMovement();
    String readStringMovement();
  private:
    uint8_t xPIN;
    uint8_t yPIN;
    int thr_x;
    int thr_y;
    String movementToSring(JoystickMovement);
};

/***************************
 *       Led Display  
 * *************************/

class LedDisplay {

  public:
    LedDisplay(uint8_t, uint8_t, uint8_t);
    void showSelectedAxis(AxisSelection);
  private:
    uint8_t ledXPIN;
    uint8_t ledYPIN;
    uint8_t ledZPIN;
};

/***************************
 *   Pedant Screen Display  
 * *************************/

class PedantScreenDisplay {

  public:
    PedantScreenDisplay(uint8_t, uint8_t, uint8_t);
    void printAxisToScreen(String);
    void printScaleToScreen(String);
    void initLCDScreen(PedantStatus);

  private:
    Adafruit_SSD1306 display;
    String xyScaleToString(xyScale);
    String zScaleToString(zScale);
    int hieght;
    int width;
    int reset;
};

#define CMD_JOG_CANCEL "\u0085"
#define JOYSTICK_SPEED "F600"

class GRBLDrivrer {

  public:
    GRBLDrivrer(uint8_t);
    void sendCommand(String);
    void performJoystickAction(JoystickMovement movement);
    int sendCommandsFromRotaryEncoder(int, PedantStatus);
    void sendCancel();

  private:
    uint8_t controlPIN;
    void recvWithEndMarker();
    const byte numChars = 32;
    char * receivedChars; // an array to store the received data
    boolean newDataArrived = false;
    bool lastJoystickReadWasNOTZero;
};

#endif