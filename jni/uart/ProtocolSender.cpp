/*
 * ProtocolSender.cpp
 *
 *  Created on: Sep 8, 2017
 *      Author: guoxs
 */

#include "uart/ProtocolSender.h"
#include "uart/UartContext.h"
#include <stdio.h>

extern BYTE getCheckSum(const BYTE *pData, int len);

/**
 * 需要根据协议格式进行拼接，以下只是个模板
 */
bool sendProtocol(const BYTE *pData, UINT16 len) {
	BYTE dataBuf[256];

	dataBuf[0] = 0xF0;
	dataBuf[1] = 0x5A;	// 同步帧头

	dataBuf[2] = HIBYTE(len);
	dataBuf[3] = LOBYTE(len);		// 数据长度

	UINT frameLen = 4;

	// 数据
	for (int i = 0; i < len; ++i) {
		dataBuf[frameLen++] = pData[i];
	}

	// 校验码
	dataBuf[frameLen++] = getCheckSum(dataBuf, frameLen);

	return UARTCONTEXT->send(dataBuf, frameLen);
}
