
#include <Arduino.h>
#include <stdint.h>

#include "config.h"
#include "thermistor.h"
#include "thermistortable.h"

#define PGM_RD_W(x)   (short)pgm_read_word(&x)



void thermitor_Init()
{
  pinMode(TH_PIN, INPUT);
}


float analog2temp() 
{
  int raw  =  0;
  float celsius = 0;
  int _pin = TH_PIN; 

  for(int j=1;j<=OVERSAMPLENR;j++){
    raw += analogRead(_pin);
  }

  if (temptable[0] != NULL) {
    float celsius = 0;
    uint8_t i;
    short(*tt)[][2] = (short(*)[][2])(temptable[0]);
    int value = (int)temptable[0];
    for (i = 1; i < value; i++) {
      if (PGM_RD_W((*tt)[i][0]) > raw) {
        celsius = PGM_RD_W((*tt)[i - 1][1]) +
                  (raw - PGM_RD_W((*tt)[i - 1][0])) *
                  (float)(PGM_RD_W((*tt)[i][1]) - PGM_RD_W((*tt)[i - 1][1])) /
                  (float)(PGM_RD_W((*tt)[i][0]) - PGM_RD_W((*tt)[i - 1][0]));
        return celsius;
      }
    }

    // Overflow: Set to last value in the table   
    if (i == (sizeof(temptable) / sizeof(temptable[0])) - 1){ 
      celsius = PGM_RD_W((*tt)[i - 1][1]);
    }
  }
  else {
    celsius = NAN;
  }

  return celsius;
}