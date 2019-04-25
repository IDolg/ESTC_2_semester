#ifndef LIGHT_CMD
#define LIGHT_CMD 1

#include "zb_common.h"
#include "zb_scheduler.h"
#include "zb_bufpool.h"
#include "zb_nwk.h"
#include "zb_aps.h"
#include "zb_zdo.h"
#include "stm32f4xx.h"
#include "../lib_set_color/SetColor.h"

#include "light_cmd.h"

/**
 *  @brief this function sends a packet 
 *  @param [in] param a buf that will be used
 *  @param [in] *data a pointer to an array to send
 *  @param [in] len an amount of symbols to send
 *  @param [in] addr send to this address
 */

void send_packet(zb_uint8_t param, zb_uint8_t *data, int len, zb_uint16_t addr)
{
    zb_buf_t *buf = ZB_BUF_FROM_REF(param);  
    zb_uint8_t *ptr = NULL;
    ZB_BUF_INITIAL_ALLOC(buf, len, ptr);
    memcpy(ptr,data,len);
    
    zb_apsde_data_req_t *req = ZB_GET_BUF_TAIL(buf, sizeof(zb_apsde_data_req_t));
    req->dst_addr.addr_short = addr;
    req->addr_mode = ZB_APS_ADDR_MODE_16_ENDP_PRESENT;
    req->tx_options = ZB_APSDE_TX_OPT_ACK_TX;
    req->radius = 1;
    req->profileid = 2;
    req->src_endpoint = 10;
    req->dst_endpoint = 10;
    buf->u.hdr.handle = 0x11;
    
    ZB_SCHEDULE_CALLBACK(zb_apsde_data_request, param);
}; 

/**
 *  @brief this function sends a command to the lamp to change its color 
 *  @param [in] param a buffer with an address of the lamp
 */

void send_cmd_change_color(zb_uint8_t param)
{
    zb_buf_t *buf = ZB_BUF_FROM_REF(param); 
    zb_uint16_t *user_parameter = ZB_GET_BUF_TAIL(buf, sizeof(zb_uint16_t));
    zb_uint16_t addr = *user_parameter;
    
    zb_uint8_t data[1];
    data[0] = CMD_CHANGE_COLOR;
    send_packet(param, data, 1, addr);
 };

/**
 *  @brief this function sends a command to the lamp to toggle 
 *  @param [in] param a buffer with an address of the lamp
 */ 
 
void send_cmd_toggle(zb_uint8_t param)
{
    zb_buf_t *buf = ZB_BUF_FROM_REF(param); 
    zb_uint16_t *user_parameter = ZB_GET_BUF_TAIL(buf, sizeof(zb_uint16_t));
    zb_uint16_t addr = *user_parameter;
    
    zb_uint8_t data[1];
    data[0] = CMD_TOGGLE;
    send_packet(param, data, 1, addr);
 }; 

/**
 *  @brief this function sends a command to the lamp to increase its brightness 
 *  @param [in] param a buffer with an address of the lamp
 */ 
 
 void send_cmd_step_up(zb_uint8_t param)
{
    zb_buf_t *buf = ZB_BUF_FROM_REF(param); 
    zb_uint16_t *user_parameter = ZB_GET_BUF_TAIL(buf, sizeof(zb_uint16_t));
    zb_uint16_t addr = *user_parameter;
    
    zb_uint8_t data[1];
    data[0] = CMD_STEP_UP;
    send_packet(param, data, 1, addr);
 }; 
 

#endif
