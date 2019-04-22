#include "../../../include/zb_types.h"

void send_packet(zb_uint8_t param, zb_uint8_t *data, int len, zb_uint16_t addr);
void send_cmd_change_color(zb_uint8_t param);
void send_cmd_toggle(zb_uint8_t param);
void send_cmd_step_up(zb_uint8_t param);

enum command {CMD_TOGGLE, CMD_STEP_UP, CMD_CHANGE_COLOR };
