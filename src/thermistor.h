#ifndef THERMISTOR_H
 #define THERMISTOR_H

 #define OVERSAMPLENR 16
 #define OV(N) int16_t((N)*(OVERSAMPLENR))

 void thermitor_Init();
 float analog2temp();

#endif