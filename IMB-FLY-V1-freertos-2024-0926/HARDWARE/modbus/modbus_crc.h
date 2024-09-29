#ifndef _modbus_crc_h_
#define _modbus_crc_h_
#include "stm32f4xx.h"
#include "sys.h"




u16  Modbus_CRC16( u8 *puchMsg, u16 usDataLen );


#endif

