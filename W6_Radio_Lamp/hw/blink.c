#ifndef BLINK_C
#define BLINK_C 1

#include "blink.h"
#include "zb_common.h"
#include "zb_scheduler.h"
#include "zb_bufpool.h"
#include "zb_nwk.h"
#include "zb_aps.h"
#include "zb_zdo.h"
#include "stm32f4xx.h"
#include "hw_init.h"

const uint32_t LED[4] = { GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15 };

void blink(void)
{
    for(int i=0; i<4; i++)
           {
            GPIO_SetBits(GPIOD, LED[i]);
            for(int j=0; j<1000000; j++);
            GPIO_ResetBits(GPIOD, LED[i]);
           }
}; 

#endif
