/***************************************************************************
*                      ZBOSS ZigBee Pro 2007 stack                         *
*                                                                          *
*          Copyright (c) 2012 DSR Corporation Denver CO, USA.              *
*                       http://www.dsr-wireless.com                        *
*                                                                          *
*                            All rights reserved.                          *
*          Copyright (c) 2011 ClarIDy Solutions, Inc., Taipei, Taiwan.     *
*                       http://www.claridy.com/                            *
*                                                                          *
*          Copyright (c) 2011 Uniband Electronic Corporation (UBEC),       *
*                             Hsinchu, Taiwan.                             *
*                       http://www.ubec.com.tw/                            *
*                                                                          *
*          Copyright (c) 2011 DSR Corporation Denver CO, USA.              *
*                       http://www.dsr-wireless.com                        *
*                                                                          *
*                            All rights reserved.                          *
*                                                                          *
*                                                                          *
* ZigBee Pro 2007 stack, also known as ZBOSS (R) ZB stack is available     *
* under either the terms of the Commercial License or the GNU General      *
* Public License version 2.0.  As a recipient of ZigBee Pro 2007 stack, you*
* may choose which license to receive this code under (except as noted in  *
* per-module LICENSE files).                                               *
*                                                                          *
* ZBOSS is a registered trademark of DSR Corporation AKA Data Storage      *
* Research LLC.                                                            *
*                                                                          *
* GNU General Public License Usage                                         *
* This file may be used under the terms of the GNU General Public License  *
* version 2.0 as published by the Free Software Foundation and appearing   *
* in the file LICENSE.GPL included in the packaging of this file.  Please  *
* review the following information to ensure the GNU General Public        *
* License version 2.0 requirements will be met:                            *
* http://www.gnu.org/licenses/old-licenses/gpl-2.0.html.                   *
*                                                                          *
* Commercial Usage                                                         *
* Licensees holding valid ClarIDy/UBEC/DSR Commercial licenses may use     *
* this file in accordance with the ClarIDy/UBEC/DSR Commercial License     *
* Agreement provided with the Software or, alternatively, in accordance    *
* with the terms contained in a written agreement between you and          *
* ClarIDy/UBEC/DSR.                                                        *
*                                                                          *
****************************************************************************
PURPOSE: Test for ZC application written using ZDO.
*/


#include "zb_common.h"
#include "zb_scheduler.h"
#include "zb_bufpool.h"
#include "zb_nwk.h"
#include "zb_aps.h"
#include "zb_zdo.h"
#include "zb_secur.h"
#include "zb_secur_api.h"

#ifndef ZB_ED_ROLE
#error define ZB_ED_ROLE to compile ze tests
#endif
/*! \addtogroup ZB_TESTS */
/*! @{ */
#ifndef ZB_SECURITY
#error Define ZB_SECURITY
#endif

enum command {ENABLE = 1, DISABLE, TOGGLE, BRIGHTNESS_UP, BRIGHTNESS_DOWN, SET_BRIGHTNESS, TOGGLE_COLOR, SET_COLOR };

struct AEP{ zb_uint16_t adress, parameter;};

static void send_command_Enable(zb_uint8_t param) ZB_CALLBACK;
static void send_command_Disable(zb_uint8_t param) ZB_CALLBACK;
static void enable_demo(zb_uint8_t param) ZB_CALLBACK;
static void disable_demo(zb_uint8_t param) ZB_CALLBACK;
static void set_brightness_demo(zb_uint8_t param) ZB_CALLBACK;
static void send_command_set_brightness(zb_uint8_t param) ZB_CALLBACK;
static void set_color_demo(zb_uint8_t param) ZB_CALLBACK;
static void send_command_set_color(zb_uint8_t param) ZB_CALLBACK;
static void send_packet(zb_uint8_t param, zb_uint8_t *data, int len, zb_uint16_t addr);
void data_indication(zb_uint8_t param) ZB_CALLBACK;
zb_ret_t dummy_d(zb_uint8_t param) ZB_CALLBACK;

zb_ieee_addr_t g_ieee_addr = {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};

MAIN()
{
  ARGV_UNUSED;

#ifndef KEIL
  if ( argc < 3 )
  {
    printf("%s <read pipe path> <write pipe path>\n", argv[0]);
    return 0;
  }
#endif

  /* Init device, load IB values from nvram or set it to default */
#ifndef ZB8051
  ZB_INIT("zdo_ze", argv[1], argv[2]);
#else
  ZB_INIT("zdo_ze", "3", "3");
#endif

  ZB_IEEE_ADDR_COPY(ZB_PIB_EXTENDED_ADDRESS(), g_ieee_addr);

  if (zdo_dev_start() != RET_OK)
  {
    TRACE_MSG(TRACE_ERROR, "zdo_dev_start failed", (FMT__0));
  }
  else
  {
    zdo_main_loop();
  }

  TRACE_DEINIT();
  MAIN_RETURN(0);
}


void zb_zdo_startup_complete(zb_uint8_t param) ZB_CALLBACK
{
  zb_buf_t *buf = ZB_BUF_FROM_REF(param);
  if (buf->u.hdr.status == 0)
  {
    TRACE_MSG(TRACE_APS1, "Device STARTED OK", (FMT__0));
//    zb_af_set_data_indication(data_indication);
    ZB_SCHEDULE_ALARM(enable_demo,0,1*ZB_TIME_ONE_SECOND);
    ZB_SCHEDULE_ALARM(disable_demo,0,2*ZB_TIME_ONE_SECOND);
    ZB_SCHEDULE_ALARM(set_brightness_demo,0,3*ZB_TIME_ONE_SECOND);
    ZB_SCHEDULE_ALARM(set_color_demo,0,4*ZB_TIME_ONE_SECOND);
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "Device started FAILED status %d", (FMT__D, (int)buf->u.hdr.status));
    zb_free_buf(buf);
  }
}


static void enable_demo(zb_uint8_t param) ZB_CALLBACK{
    if (param == 0)
    {
        ZB_GET_OUT_BUF_DELAYED(enable_demo);
        return;
    }
    
    zb_buf_t *buf = ZB_BUF_FROM_REF(param);
    *ZB_GET_BUF_PARAM(buf, zb_uint16_t) = 0; //addr
    ZB_SCHEDULE_CALLBACK(send_command_Enable, param);
}

static void disable_demo(zb_uint8_t param) ZB_CALLBACK{
    if (param == 0)
    {
        ZB_GET_OUT_BUF_DELAYED(disable_demo);
        return;
    }
    
    zb_buf_t *buf = ZB_BUF_FROM_REF(param);
    *ZB_GET_BUF_PARAM(buf, zb_uint16_t) = 0; //addr
    ZB_SCHEDULE_CALLBACK(send_command_Disable, param);
}

static void set_brightness_demo(zb_uint8_t param) ZB_CALLBACK{
    if (param == 0)
    {
        ZB_GET_OUT_BUF_DELAYED(set_brightness_demo);
        return;
    }
    
    zb_buf_t *buf = ZB_BUF_FROM_REF(param);  
    struct AEP *aep = ZB_GET_BUF_TAIL(buf, sizeof(struct AEP));
    aep->adress = 0;
    aep->parameter = 15;
    ZB_SCHEDULE_CALLBACK(send_command_set_brightness, param);
}

static void set_color_demo(zb_uint8_t param) ZB_CALLBACK{
    if (param == 0)
    {
        ZB_GET_OUT_BUF_DELAYED(set_color_demo);
        return;
    }
    
    zb_buf_t *buf = ZB_BUF_FROM_REF(param);  
    struct AEP *aep = ZB_GET_BUF_TAIL(buf, sizeof(struct AEP));
    aep->adress = 0;
    aep->parameter = 6;
    ZB_SCHEDULE_CALLBACK(send_command_set_color, param);
}

static void send_packet(zb_uint8_t param, zb_uint8_t *data, int len, zb_uint16_t addr) {
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
}

void send_command_Enable(zb_uint8_t param) ZB_CALLBACK{
    zb_buf_t *buf = ZB_BUF_FROM_REF(param); 
    zb_uint16_t *user_parameter = ZB_GET_BUF_TAIL(buf, sizeof(zb_uint16_t));
    zb_uint16_t addr = *user_parameter;
    
    zb_uint8_t data[1];
    data[0] = ENABLE;
    send_packet(param, data, 1, addr);
 }

void send_command_Disable(zb_uint8_t param) ZB_CALLBACK{
    zb_buf_t *buf = ZB_BUF_FROM_REF(param); 
    zb_uint16_t *user_parameter = ZB_GET_BUF_TAIL(buf, sizeof(zb_uint16_t));
    zb_uint16_t addr = *user_parameter;
    
    zb_uint8_t data[1];
    data[0] = DISABLE;
    send_packet(param, data, 1, addr);
 }
 
void send_command_set_brightness(zb_uint8_t param) ZB_CALLBACK{
    zb_buf_t *buf = ZB_BUF_FROM_REF(param); 
    struct AEP *aep = ZB_GET_BUF_TAIL(buf, sizeof(struct AEP));
    zb_uint16_t addr = aep->adress;
    zb_uint16_t parameter = aep->parameter;
    
    zb_uint8_t data[2];
    data[0] = SET_BRIGHTNESS;
    data[1] = parameter;
    send_packet(param, data, 2, addr);
 }
 
void send_command_set_color(zb_uint8_t param) ZB_CALLBACK{
    zb_buf_t *buf = ZB_BUF_FROM_REF(param); 
    struct AEP *aep = ZB_GET_BUF_TAIL(buf, sizeof(struct AEP));
    zb_uint16_t addr = aep->adress;
    zb_uint16_t parameter = aep->parameter;
    
    zb_uint8_t data[2];
    data[0] = SET_COLOR;
    data[1] = parameter;
    send_packet(param, data, 2, addr);
 }

/*! @} */
