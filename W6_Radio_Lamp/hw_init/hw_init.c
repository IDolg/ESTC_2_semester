#ifndef HW_INIT
#define HW_INIT 1

#include "zb_common.h"
#include "zb_scheduler.h"
#include "zb_bufpool.h"
#include "zb_nwk.h"
#include "zb_aps.h"
#include "zb_zdo.h"
#include "stm32f4xx.h"
#include "../lib_set_color/SetColor.h"

#include "hw_init.h"

const uint32_t LED[4] = { GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15 };
const uint32_t LEDs = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ; 

void blink(void){
    for(int i=0; i<4; i++)
           {
            GPIO_SetBits(GPIOD, LED[i]);
            for(int j=0; j<1000000; j++);
            GPIO_ResetBits(GPIOD, LED[i]);
           }
};

void init_LED(void){
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_InitTypeDef A;
  A.GPIO_Pin = LEDs ;
  A.GPIO_Mode  = GPIO_Mode_OUT;
  A.GPIO_OType = GPIO_OType_PP;
  A.GPIO_Speed = GPIO_Speed_100MHz;
  A.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &A);
};

void InitButtons(void){
 
  GPIO_InitTypeDef GPIO_InitStructure;
    
/* Enable peripheral clock for button port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  /* Init button */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
};

void InitButtonInterruptions(void){
// 1 button  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);  
  EXTI_InitTypeDef EXTI_InitStruct;
  EXTI_InitStruct.EXTI_Line = EXTI_Line1;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_Init(&EXTI_InitStruct);
  
  NVIC_EnableIRQ(EXTI1_IRQn);
  
  NVIC_InitTypeDef NVIC_InitStruct;
  NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
// 2 button 
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource2);  
  EXTI_InitStruct.EXTI_Line = EXTI_Line2;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_Init(&EXTI_InitStruct);
  
  NVIC_EnableIRQ(EXTI2_IRQn);
  
  NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);   
};

#endif
