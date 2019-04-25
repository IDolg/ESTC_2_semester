#ifndef LIGHT_CMD_H
#define LIGHT_CMD_H 1

#include "../../../include/zb_types.h"

/**
  * @file    light_cmd.h  
  * @author  Igor Dolgih
  * @version V1.1.0
  * @date    25-April-2019
  * @brief   Functions that send commands to remote lamp.
  *  The command contains one char symbol. 
  *  "0" means "toggle the lamp"
  *  "1" means "step up the brightness"
  *  "2" means "change the color"
 */

void send_packet(zb_uint8_t param, zb_uint8_t *data, int len, zb_uint16_t addr);
void send_cmd_change_color(zb_uint8_t param);
void send_cmd_toggle(zb_uint8_t param);
void send_cmd_step_up(zb_uint8_t param);
 

enum command {CMD_TOGGLE, CMD_STEP_UP, CMD_CHANGE_COLOR };

#endif
