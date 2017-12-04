/*
 * ZKQRCode.h
 *
 *  Created on: Oct 9, 2017
 *      Author: guoxs
 */

#ifndef _CONTROL_ZKQRCODE_H_
#define _CONTROL_ZKQRCODE_H_

#include "ZKBase.h"

class ZKQRCode : public ZKBase {
public:
	ZKQRCode(HWND hParentWnd);
	virtual ~ZKQRCode();

	bool loadQRCode(const char *pStr);

protected:
	virtual void onBeforeCreateWindow(const Json::Value &json);
	virtual const char* getClassName() const { return ZK_QRCODE; }
	virtual void onDraw(HDC hdc);

	void drawQRCode(HDC hdc);

private:
	void parseQRCodeAttributeFromJson(const Json::Value &json);
	void deleteQRCodeDC();

private:
	HDC mQRCodeDC;
};

#endif /* _CONTROL_ZKQRCODE_H_ */
