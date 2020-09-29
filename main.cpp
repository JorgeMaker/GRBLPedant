#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define JOYSTICK_SPEED "F600"

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define X_BUTTON_PIN 36
#define Y_BUTTON_PIN 39
#define Z_BUTTON_PIN 34
#define STOP_BUTTON_PIN 35
#define SCALE_BUTTON_PIN 23

#define ROTARY_MOV_S1 5
#define ROTARY_MOV_S2 4

#define X_LED_PIN 32
#define Y_LED_PIN 19
#define Z_LED_PIN 33

#define CONTROL_SERIAL_PIN 13

#define JOYSTICK_VRX_PIN 26
#define JOYSTICK_VRY_PIN 27


#define STATE_X 1
#define STATE_Y 2
#define STATE_Z 3

#define INCREMENT_SCALE 1
#define DECREMENT_SCALE 0

#define CMD_JOG_CANCEL "\u0085"


unsigned long rotaryMoveLastChange = 0;
unsigned long buttonScaleLastChange = 0;
int pedantState = STATE_X;

int zScalePosition = 2;

int xScalePosition = 2;
int yScalePosition = 2;

boolean cancelFlag = false;
String zMovement = "0";
String previousZMovement = "0";

String xMovement = "0";
String previousXMovement = "0";

String yMovement = "0";
String previousYMovement = "0";

String xyScalePositionToString(int position)
{
  switch (position)
  {
  case 0: // 0,1 mm
    return "0.1";
    break;
  case 1: // 0,5 mm
    return "0.5";
    break;
  case 2: //  1 mm
    return "1.0";
    break;
  case 3: //  10mm
    return " 10";
    break;
  case 4: //  50 mm
    return " 50";
    break;
  case 5: //  100 mm
    return "100";
    break;
  default:
    return "0.1";
    break;
  }
}

void modifyXScalePosition(int operation)
{
  if (operation == INCREMENT_SCALE)
  {
    if (xScalePosition < 5)
      xScalePosition++;
    else
    {
      xScalePosition = 0;
    }
  }
  if (operation == DECREMENT_SCALE)
  {
    if (xScalePosition > 0)
      xScalePosition--;
    else
    {
      xScalePosition = 5;
    }
  }
}

void modifyYScalePosition(int operation)
{
  if (operation == INCREMENT_SCALE)
  {
    if (yScalePosition < 5)
      yScalePosition++;
    else
    {
      yScalePosition = 0;
    }
  }
  if (operation == DECREMENT_SCALE)
  {
    if (yScalePosition > 0)
      yScalePosition--;
    else
    {
      yScalePosition = 5;
    }
  }
}

String zScalePositionToString(int position)
{
  switch (position)
  {
  case 0: // 0,1 mm
    return "0.1";
    break;
  case 1: // 0,5 mm
    return "0.5";
    break;
  case 2: //  1 mm
    return "1.0";
    break;
  case 3: //  10mm
    return " 10";
    break;
  case 4: //  50 mm
    return " 50";
    break;
  default:
    return "0.1";
    break;
  }
}
void modifyZScalePosition(int operation)
{
  if (operation == INCREMENT_SCALE)
  {
    if (zScalePosition < 4)
      zScalePosition++;
    else
    {
      zScalePosition = 0;
    }
  }
  if (operation == DECREMENT_SCALE)
  {
    if (zScalePosition > 0)
      zScalePosition--;
    else
    {
      zScalePosition = 4;
    }
  }
}

struct PedantButtons
{
  int buttonX;
  int buttonY;
  int buttonZ;
};
struct PedantButtons pedantButtons;

struct PedantButtons readPedantButtons(int buttonX_pin, int buttonY_pin, int buttonZ_pin)
{

  PedantButtons buttons;

  buttons.buttonX = digitalRead(buttonX_pin);
  buttons.buttonY = digitalRead(buttonY_pin);
  buttons.buttonZ = digitalRead(buttonZ_pin);

  return buttons;
}

int numOfPressedButons()
{
  int sum = 0;
  if (pedantButtons.buttonX)
  {
    sum++;
  }
  if (pedantButtons.buttonY)
  {
    sum++;
  }
  if (pedantButtons.buttonZ)
  {
    sum++;
  }
  return sum;
}
void setLedState(int led)
{
  if (led == STATE_X)
  {
    digitalWrite(X_LED_PIN, HIGH);
    digitalWrite(Y_LED_PIN, LOW);
    digitalWrite(Z_LED_PIN, LOW);
  }
  else if (led == STATE_Y)
  {
    digitalWrite(X_LED_PIN, LOW);
    digitalWrite(Y_LED_PIN, HIGH);
    digitalWrite(Z_LED_PIN, LOW);
  }
  else if (led == STATE_Z)
  {
    digitalWrite(X_LED_PIN, LOW);
    digitalWrite(Y_LED_PIN, LOW);
    digitalWrite(Z_LED_PIN, HIGH);
  }
}
void printAxisToScreen(String axis)
{
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(3);
  display.setCursor(90, 19);
  display.print(axis);
  display.display();
}
void readState()
{
  pedantButtons = readPedantButtons(X_BUTTON_PIN, Y_BUTTON_PIN, Z_BUTTON_PIN);
  int pressedButons = numOfPressedButons();
  if (pressedButons == 0)
  {
    return;
  }
  else if (pressedButons == 1)
  {
    if (pedantButtons.buttonX == HIGH)
    {
      pedantState = STATE_X;
      setLedState(STATE_X);
      printAxisToScreen("X");
    }
    else if (pedantButtons.buttonY == HIGH)
    {
      pedantState = STATE_Y;
      setLedState(STATE_Y);
      printAxisToScreen("Y");
    }
    else if (pedantButtons.buttonZ == HIGH)
    {
      pedantState = STATE_Z;
      setLedState(STATE_Z);
      printAxisToScreen("Z");
    }
  }
}

void attendMovRotaryEncoder()
{
  unsigned long currentMillis = millis();
  if ((currentMillis - rotaryMoveLastChange) < 20)
  {
    return;
  }
  else
  {
    rotaryMoveLastChange = currentMillis;
    // determine Axis to make the movement and direction
    if (pedantState == STATE_X)
    {
      if (digitalRead(ROTARY_MOV_S1) && !digitalRead(ROTARY_MOV_S2))
      {
        xMovement = "1";
      }
      if (digitalRead(ROTARY_MOV_S1) && digitalRead(ROTARY_MOV_S2))
      {
        xMovement = "-1";
      }
    }
    else if (pedantState == STATE_Y)
    {
      if (digitalRead(ROTARY_MOV_S1) && !digitalRead(ROTARY_MOV_S2))
      {
        yMovement = "1";
      }
      if (digitalRead(ROTARY_MOV_S1) && digitalRead(ROTARY_MOV_S2))
      {
        yMovement = "-1";
      }
    }
    else if (pedantState == STATE_Z)
    {
      if (digitalRead(ROTARY_MOV_S1) && !digitalRead(ROTARY_MOV_S2))
      {
        zMovement = "1";
      }
      if (digitalRead(ROTARY_MOV_S1) && digitalRead(ROTARY_MOV_S2))
      {
        zMovement = "-1";
      }
    }
  }
}
void readScaleButton()
{
  unsigned long currentMillis = millis();
  if ((currentMillis - buttonScaleLastChange) < 200)
  {
    return;
  }
  else
  {
    buttonScaleLastChange = currentMillis;
    if (pedantState == STATE_X)
    {
      modifyXScalePosition(INCREMENT_SCALE);
    }
    if (pedantState == STATE_Y)
    {
      modifyYScalePosition(INCREMENT_SCALE);
    }
    else if (pedantState == STATE_Z)
    {
      modifyZScalePosition(INCREMENT_SCALE);
    }
  }
}

void initRotary()
{
  pinMode(ROTARY_MOV_S1, INPUT);
  pinMode(ROTARY_MOV_S2, INPUT);

  attachInterrupt(digitalPinToInterrupt(ROTARY_MOV_S1), attendMovRotaryEncoder, RISING);
}

void printScaleToScreen(String scaleString)
{
  display.setTextSize(2);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(80, 45);
  display.print(scaleString);
}

void updateScaleIntoScreen()
{
  if (pedantState == STATE_X)
  {
    printScaleToScreen(xyScalePositionToString(xScalePosition));
  }
  if (pedantState == STATE_Y)
  {
    printScaleToScreen(xyScalePositionToString(yScalePosition));
  }
  else if (pedantState == STATE_Z)
  {
    printScaleToScreen(zScalePositionToString(zScalePosition));
  }
  display.display();
}

void initLCDScreen()
{
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
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

  display.display();
  updateScaleIntoScreen();
  printAxisToScreen("X");
}
void initScaleButton()
{
  Serial.println(F("Initiating Scale Button"));
  pinMode(SCALE_BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(SCALE_BUTTON_PIN), readScaleButton, FALLING);
}
void cancelButtonAction()
{
  cancelFlag = true;
}

void initCancelButton()
{
  pinMode(STOP_BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(STOP_BUTTON_PIN), cancelButtonAction, CHANGE);
}
void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);

  pinMode(X_BUTTON_PIN, INPUT);
  pinMode(Y_BUTTON_PIN, INPUT);
  pinMode(Z_BUTTON_PIN, INPUT);

  pinMode(X_LED_PIN, OUTPUT);
  pinMode(Y_LED_PIN, OUTPUT);
  pinMode(Z_LED_PIN, OUTPUT);

  pinMode(CONTROL_SERIAL_PIN, OUTPUT);

  digitalWrite(X_LED_PIN, LOW);
  digitalWrite(X_LED_PIN, LOW);
  digitalWrite(X_LED_PIN, LOW);

  initRotary();
  initLCDScreen();
  initScaleButton();
  initCancelButton();
  setLedState(STATE_X);
}
void grblCmdSend(String txCmd)
{
  digitalWrite(CONTROL_SERIAL_PIN, HIGH);
  delay(50);
  Serial2.print(txCmd + '\r');
  delay(50);
  digitalWrite(CONTROL_SERIAL_PIN, LOW);
}

const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data
boolean newDataArrived = false;

void recvWithEndMarker()
{
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  while (Serial2.available() > 0 && newDataArrived == false)
  {
    rc = Serial2.read();
    if (rc != endMarker)
    {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars)
      {
        ndx = numChars - 1;
      }
    }
    else
    {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newDataArrived = true;
    }
  }
}
boolean lastJoystickReadWasNOTZero = false;

void readJoystick()
{
  int vrX = analogRead(JOYSTICK_VRX_PIN);
  if (vrX > 3000)
  {
    grblCmdSend("G91 G0 X1 F1800");
    recvWithEndMarker();
    newDataArrived = false;
    lastJoystickReadWasNOTZero = true;
  }
  else if (vrX < 1500)
  {
    grblCmdSend("G91 G0 X-1 F1800");
    recvWithEndMarker();
    newDataArrived = false;
    lastJoystickReadWasNOTZero = true;
  }
  else
  {
    if (lastJoystickReadWasNOTZero)
    {
      grblCmdSend(CMD_JOG_CANCEL);
      recvWithEndMarker();
    }
    lastJoystickReadWasNOTZero = false;
    // TODO TODO TODO Arreglar el tema del cancelar al pasar por 0
  }
  int vrY = analogRead(JOYSTICK_VRY_PIN);

  if (vrY > 3000)
  {
    grblCmdSend("G91 G0 Y1 F1800");
    recvWithEndMarker();
    newDataArrived = false;
    lastJoystickReadWasNOTZero = true;
  }
  else if (vrY < 1500)
  {
    grblCmdSend("G91 G0 Y-1 F1800");
    recvWithEndMarker();
    newDataArrived = false;
    lastJoystickReadWasNOTZero = true;
  }
}

void executeCommands()
{
  if (cancelFlag)
  {
    grblCmdSend(CMD_JOG_CANCEL);
    cancelFlag = false;
    return;
  }
  String xScale = xyScalePositionToString(xScalePosition);
  String yScale = xyScalePositionToString(yScalePosition);
  if (xMovement == "1")
  {
    if (previousXMovement == "-1")
    {
      grblCmdSend(CMD_JOG_CANCEL);
    }
    grblCmdSend("$J=G21G91X" + xScale + JOYSTICK_SPEED);
    previousXMovement = xMovement;
    xMovement = "0";
  }
  else if (xMovement == "-1")
  {
    if (previousXMovement == "1")
    {
      grblCmdSend(CMD_JOG_CANCEL);
    }
    grblCmdSend("$J=G21G91X-" + xScale + JOYSTICK_SPEED);
    previousXMovement = xMovement;
    xMovement = "0";
  }
  else if (yMovement == "1")
  {
    if (previousYMovement == "-1")
    {
      grblCmdSend(CMD_JOG_CANCEL);
    }
    grblCmdSend("$J=G21G91Y" + yScale + JOYSTICK_SPEED);
    previousYMovement = yMovement;
    yMovement = "0";
  }
  else if (yMovement == "-1")
  {
    if (previousYMovement == "1")
    {
      grblCmdSend(CMD_JOG_CANCEL);
    }
    grblCmdSend("$J=G21G91Y-" + yScale + JOYSTICK_SPEED);
    previousXMovement = xMovement;
    yMovement = "0";
  }
  String zScale = zScalePositionToString(zScalePosition);
  if (zMovement == "1")
  {
    if (previousZMovement == "-1")
    {
      grblCmdSend(CMD_JOG_CANCEL);
    }

    grblCmdSend("$J=G21G91Z" + zScale + JOYSTICK_SPEED);
    previousZMovement = zMovement;
    zMovement = "0";
  }
  else if (zMovement == "-1")
  {
    if (previousZMovement == "1")
    {
      grblCmdSend(CMD_JOG_CANCEL);
    }

    grblCmdSend("$J=G21G91Z-" + zScale + JOYSTICK_SPEED);
    previousZMovement = zMovement;
    zMovement = "0";
  }
}
void loop()
{
  readState();
  updateScaleIntoScreen();
  readJoystick();
  executeCommands();
}