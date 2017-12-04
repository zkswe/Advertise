/*
 * ZKCheckBox.h
 *
 *  Created on: Oct 22, 2017
 *      Author: guoxs
 */

#ifndef _CONTROL_ZKCHECKBOX_H_
#define _CONTROL_ZKCHECKBOX_H_

#include "ZKButton.h"

class ZKCheckBox : public ZKButton {
public:
	ZKCheckBox(HWND hParentWnd);
	virtual ~ZKCheckBox();

	void setChecked(bool isChecked);
	bool isChecked() const;

public:
	class ICheckedChangeListener {
	public:
		virtual ~ICheckedChangeListener() { }
		virtual void onCheckedChanged(ZKCheckBox *pCheckBox, bool isChecked) = 0;
	};

	void setCheckedChangeListener(ICheckedChangeListener *pListener) {
		mCheckedChangeListenerPtr = pListener;
	}

protected:
	virtual void onBeforeCreateWindow(const Json::Value &json);
	virtual const char* getClassName() const { return ZK_CHECKBOX; }

	virtual void onDraw(HDC hdc);
	virtual BOOL onTouchEvent(const MotionEvent &ev);

	void drawBox(HDC hdc);

private:
	void parseCheckBoxAttributeFromJson(const Json::Value &json);

private:
	ICheckedChangeListener *mCheckedChangeListenerPtr;
	LayoutPosition mBoxPosition;
};

#endif /* _CONTROL_ZKCHECKBOX_H_ */
