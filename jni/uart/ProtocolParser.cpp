/*
 * ProtocolParser.cpp
 *
 *  Created on: Sep 7, 2017
 *      Author: guoxs
 */
#include <vector>
#include "CommDef.h"
#include "uart/ProtocolParser.h"
#include "utils/Log.h"

/* SynchFrame CmdID  DataLen Data CheckSum */
/*     2Byte  2Byte   1Byte	N Byte  1Byte */
// 最小长度: 2 + 2 + 1 + 1= 6

#define DATA_PACKAGE_MIN_LEN		6
#define CMD_HEAD1	0xFF
#define CMD_HEAD2	0x55

static std::vector<OnProtocolDataUpdateFun> sProtocolDataUpdateListenerList;

void registerProtocolDataUpdateListener(OnProtocolDataUpdateFun pListener) {
	LOGD("registerProtocolDataUpdateListener\n");
	if (pListener != NULL) {
		sProtocolDataUpdateListenerList.push_back(pListener);
	}
}

void unregisterProtocolDataUpdateListener(OnProtocolDataUpdateFun pListener) {
	LOGD("unregisterProtocolDataUpdateListener\n");
	if (pListener != NULL) {
		std::vector<OnProtocolDataUpdateFun>::iterator iter = sProtocolDataUpdateListenerList.begin();
		for (; iter != sProtocolDataUpdateListenerList.end(); iter++) {
			if ((*iter) == pListener) {
				sProtocolDataUpdateListenerList.erase(iter);
				return;
			}
		}
	}
}

static void notifyProtocolDataUpdate(const SProtocolData &data) {
	std::vector<OnProtocolDataUpdateFun>::const_iterator iter = sProtocolDataUpdateListenerList.begin();
	for (; iter != sProtocolDataUpdateListenerList.end(); iter++) {
		(*iter)(data);
	}
}

static SProtocolData sProtocolData = { 0 };

SProtocolData& getProtocolData() {
	return sProtocolData;
}

/**
 * 获取校验码
 */
BYTE getCheckSum(const BYTE *pData, int len) {
	int sum = 0;
	for (int i = 0; i < len; ++i) {
		sum += pData[i];
	}

	return (BYTE) (~sum + 1);
}

/**
 * 解析每一帧数据
 */
static void procParse(const BYTE *pData, UINT len) {
	switch (MAKEWORD(pData[1], pData[0])) {
	case CMDID_POWER:
		sProtocolData.power = pData[4];
		break;
	}

	// 通知协议数据更新
	notifyProtocolDataUpdate(sProtocolData);
}

/**
 * 功能：解析协议
 * 参数：pData 协议数据，len 数据长度
 * 返回值：实际解析协议的长度
 */
int parseProtocol(const BYTE *pData, UINT len) {
	UINT remainLen = len;	// 剩余数据长度
	UINT dataLen;	// 数据包长度
	UINT frameLen;	// 帧长度

	/**
	 * 以下部分需要根据协议格式进行相应的修改，解析出每一帧的数据
	 */
	while (remainLen >= DATA_PACKAGE_MIN_LEN) {
		// 找到一帧数据的数据头
		while ((remainLen >= 2) && ((pData[0] != CMD_HEAD1) || (pData[1] != CMD_HEAD2))) {
			pData++;
			remainLen--;
			continue;
		}

		if (remainLen < DATA_PACKAGE_MIN_LEN) {
			break;
		}

		dataLen = MAKEWORD(pData[5], pData[4]);
		frameLen = dataLen + DATA_PACKAGE_MIN_LEN;
		if (frameLen > remainLen) {
			// 数据内容不全
			break;
		}

#ifdef DEBUG_PRO_DATA
		for (int i = 0; i < frameLen; ++i) {
			LOGD("%x ", pData[i]);
		}
		LOGD("\n");
#endif

		// 检测校验码
		if (getCheckSum(pData, frameLen - 1) == pData[frameLen - 1]) {
			// 解析一帧数据
			procParse(pData, frameLen);
		} else {
			LOGE("CheckSum error!!!!!!\n");
		}

		pData += frameLen;
		remainLen -= frameLen;
	}

	return len - remainLen;
}
