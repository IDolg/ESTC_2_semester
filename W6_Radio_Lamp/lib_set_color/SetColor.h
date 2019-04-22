#ifndef _LED_H_
#define _LED_H_

#include <stdint.h>
#include <stm32f4xx.h>
/**
  * @file    SetColor.h  
  * @author  Igor Dolgih
  * @version V1.1.0
  * @date    15-December-2018
  * @brief   Library that controls color
 */
void InitLEDs(void);
void InitTimer(void);
void InitPWM(void);
void AddRed(void);
void AddGreen(void);
void AddBlue(void);
void SetColor(int R, int G, int B);
uint8_t ChooseColor(uint8_t led_number);
void AddThisColor(uint8_t led_number);

void StepUp(void);

#endif
