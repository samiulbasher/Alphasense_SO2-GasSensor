#ifndef config_h
 #define config_h

 #define BAUDRATE 115200

 #define DISPLAY_TEMPERATURE

 #define LCD_WIDTH  16
 #define LCD_HEIGHT 2

 #define LCD_RS 5
 #define LCD_EN 6
 #define LCD_D4 7
 #define LCD_D5 8
 #define LCD_D6 9
 #define LCD_D7 10
 
 /*!
  # Alphasence SO2 sensor (use UART mode)
  * @ Alphasence ISB Board 6 pin connector
  * @ + pin    = VCC (5v)
  * @ - pin    = GND
  * @ OP1 +pin = A0 (WE pin) 
  * @ OP1 -pin = GND
  * @ OP2 +pin = A1 (Aux pin) 
  * @ OP2 -pin = GND
 */
 #define SO2_WE  A0  //Working Electrode
 #define SO2_AUX A1  //Auxiliary Electrode

 #define DBG_LED  13
 #define BUZZ_PIN A3
 #define ESW_PIN  A2
 #define TH_PIN   A7

 #define LONG_BEEP 0
 #define SHORT_BEEP 1

 int dataSelection(int a[], int n);
 void beeper(uint8_t numOfBeep, uint8_t beepPattern);

 // Macros to support option testing
 #define _CAT(a, ...) a ## __VA_ARGS__   
 #define SWITCH_ENABLED_      1
 #define ENABLED(b) _CAT(SWITCH_ENABLED_, b)

#endif
