/*
**
**                           debugLeds.h
**
**
**********************************************************************/

#ifndef __DEBUGLEDS_H
#define __DEBUGLEDS_H

void debugBlinkBlueLed(const int divScaler);
void debugBlinkRedLed(const int divScaler);
void debugBlinkOrangeLed(const int divScaler);
void debugBlinkGreenLed(const int divScaler);
void initGPIO_LED(void);

#endif /* __DEBUGLEDS_H */

