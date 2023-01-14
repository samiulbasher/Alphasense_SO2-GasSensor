#include <Arduino.h>
#include <LiquidCrystal.h>

#include <stdint.h>

#include "config.h"
#include "thermistor.h"


LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);


// Sensor SO2-B4 SN: 164781758 (according to alphasense provided calibration data)
float ElecZero_SO2_WE_mV = 362;  //[usually:330mV] the elctronic zero of the SO2 working electrode. Basically the baseline for 0ppb, see write up for alternative values
float ElecZero_SO2_Aux_mV = 341; //[usually:340mV] similarly , the electronic zero of the auxialary electrode, which is isolated from air to give unbiased values for stability
float SO2_Sensitivity = 0.300;   //[usually:300nA/ppm] the sensitivity of the SO2 sensor in (mV/ppb)


float Vin = 4980;//4720;  //This is the input voltage(mV), that we want to be stable, using bypass capacitors and a linear regulator. its better to do not use the Arduino as a voltage source.

uint8_t samples = 60; // number of samples taken to determine the mode in mode function
int ppb_SO2[60]; //arrays holding multiple data samples, used in mode function to pick reoccuring value


unsigned long previous_time = 0;
unsigned long wait_updateTime = 1000;    //we will check the sensor 1 times a second

void setup() {
  // put your setup code here, to run once:

  Serial.begin(BAUDRATE);
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);

  //Define analog pins as input readers for incoming voltage values
  pinMode(SO2_WE,  INPUT); //SO2 WE
  pinMode(SO2_AUX, INPUT); //SO2 AUX

  pinMode(DBG_LED, OUTPUT);
  pinMode(BUZZ_PIN, OUTPUT);

  thermitor_Init();

  Serial.println(">start");
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < samples; i++) //this for loop will take data points every 10ms, statisticall it is redommended to have at least 50 data points to choose a mode or average from, we use n = 60
  {
    delay(10); //Analog to Digital conversion can happen up to 10,000 times per second on the arduino, but a 10ms delay is easier on the machine.

    // read the input on analog pin A0, A1:
    int SO2_WE_Value  = analogRead(SO2_WE);  //this is the SO2 Working Eletrode value (0-1023)
    int SO2_AUX_Value = analogRead(SO2_AUX); //this is the Auxialary electrode, which is not exposed to any air to be used as a reference for stability.

    /*
    Serial.print("SO2 Working Eletrode: ");
    Serial.println(SO2_WE_Value);
    Serial.print("SO2 Auxialary electrode: ");
    Serial.println(SO2_AUX_Value);
    */

   
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5000mV), and correct by subtacting the electronic zero(mV) of each, giving us the real mV of each:
    float SO2_WE_mV  = (SO2_WE_Value  * (Vin / 1023.0)) - ElecZero_SO2_WE_mV;
    float SO2_AUX_mV = (SO2_AUX_Value * (Vin / 1023.0)) - ElecZero_SO2_Aux_mV;


    //take the difference of the working and auxialry electrode voltages to get the actual voltage
    float dV_SO2 = SO2_WE_mV - SO2_AUX_mV;

    //Serial.print("Difference: ");
    //Serial.println(dV_SO2);

    //divide this value by sensitivity to get parts per billion (since dV is in mV and sensitivity is in (mV/ppb), so we work with the reciprical 1/sensitivity to get ppb/mV
    ppb_SO2[i] = dV_SO2 / SO2_Sensitivity;
  }

  digitalWrite(DBG_LED,HIGH);

  int SO2 = dataSelection(ppb_SO2, samples);
  if(SO2 < 0) SO2 = 0;
  String gas = " SO2:" + String(SO2) + "ppb ";

  lcd.setCursor((LCD_WIDTH - gas.length()) / 2, 0); //center the lcd text
  lcd.print(gas);
  Serial.println(gas);

  
  #if ENABLED(DISPLAY_TEMPERATURE)
    // read thermistor and display in 2nd line
    if((millis() - previous_time) > wait_updateTime) //check every 1 times in a secound
    {
      previous_time = millis();
      String temperature = "R.Temp: " + String(round(analog2temp())) + (char)223 +"C";

      lcd.setCursor((LCD_WIDTH - temperature.length()) / 2, 1); //center the lcd text
      lcd.print(temperature);
      //Serial.println(temperature);
    }
  #endif //DISPLAY_TEMPERATURE

  digitalWrite(DBG_LED,LOW);
}

//This function to find the most common value out of "samples" trails to display, instead of average that looses resolution intervals
int dataSelection(int a[], int n) {
  int maxValue = 0, maxCount = 0, i, j;
  for (i = 0; i < n; ++i) {
    int count = 0;
    for (j = 0; j < n; ++j) {
      if (a[j] == a[i])
        ++count;
    }
    if (count > maxCount) {
      maxCount = count;
      maxValue = a[i];
    }
  }
  return maxValue;
}

void beeper(uint8_t numOfBeep, uint8_t beepPattern)
{
  uint16_t beepDelay;
  
  if(beepPattern == SHORT_BEEP)  beepDelay = 10;
  if(beepPattern == LONG_BEEP )  beepDelay = 300;

  for (int i = 0; i < numOfBeep; i++)
  {
    digitalWrite(BUZZ_PIN, HIGH);
    delay(beepDelay);
    digitalWrite(BUZZ_PIN, LOW);
    delay(beepDelay);
  }
}
