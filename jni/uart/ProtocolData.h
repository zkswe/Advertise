/*
 * ProtocolData.h
 *
 *  Created on: Sep 7, 2017
 *      Author: guoxs
 */

#ifndef _UART_PROTOCOL_DATA_H_
#define _UART_PROTOCOL_DATA_H_

#include <string>
#include "CommDef.h"

/* CmdID	DeviceID	DataLen		Data */
/* 2Byte     1Byte      1Byte      N Byte*/

/******************** CmdID ***********************/
#define CMDID_POWER							0x0
#define CMDID_RUN_MODE						0x1
/**************************************************/

/******************** 错误码 ***********************/
#define ERROR_CODE_CMDID			1
/**************************************************/

typedef struct {
	UINT16 slaveAddr;		// 从机地址
	UINT16 masterAddr;		// 主机地址
	BYTE power;
} SProtocolData;

#endif /* _UART_PROTOCOL_DATA_H_ */
