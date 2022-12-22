#ifndef config_h
 #define config_h

 /*!
  ### pin connection

  # LCD(16x2) with module
  * @ GND = GND
  * @ VCC = VCC (5v)
  * @ SDA = SDA
  * @ SCL = SCL
 */
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

 #define DBG_LED 13

 #define BAUDRATE 115200

 int dataSelection(int a[], int n);

#endif