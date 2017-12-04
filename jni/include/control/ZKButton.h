/*
 * ZKButton.h
 *
 *  Created on: Jun 10, 2017
 *      Author: guoxs
 */

#ifndef _CONTROL_ZKBUTTON_H_
#define _CONTROL_ZKBUTTON_H_

#include "ZKTextView.h"

class ZKButton : public ZKTextView {
public:
	ZKButton(HWND hParentWnd);
	virtual ~ZKButton();

protected:
	virtual void onBeforeCreateWindow(const Json::Value &json);
	virtual const char* getClassName() const { return ZK_BUTTON; }

	virtual void onDraw(HDC hdc);

	void _section_(zk) drawButton(HDC hdc);

	const PBITMAP getCurStatusPic() const;

private:
	void _section_(zk) parseButtonAttributeFromJson(const Json::Value &json);

protected:
	PBITMAP mButtonStatusPicTab[S_CONTROL_STATUS_TAB_LEN];
};

#endif /* _CONTROL_ZKBUTTON_H_ */
