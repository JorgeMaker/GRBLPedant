#ifndef CONFIG_H
#define CONFIG_H

#define X_BUTTON_PIN 36
#define Y_BUTTON_PIN 39
#define Z_BUTTON_PIN 34
#define STOP_BUTTON_PIN 35
#define SCALE_BUTTON_PIN 23

#define JOYSTICK_VRX_PIN 26
#define JOYSTICK_VRY_PIN 27

#define THRESHOLD_X 1500
#define THRESHOLD_Y 1500

#define X_LED_PIN 32
#define Y_LED_PIN 19
#define Z_LED_PIN 33

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET - 1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define CONTROL_SERIAL_PIN 13

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET - 1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define ROTARY_MOV_S1 5
#define ROTARY_MOV_S2 4

#endif