/*
 * ZKEditText.h
 *
 *  Created on: Oct 27, 2017
 *      Author: guoxs
 */

#ifndef _CONTROL_ZKEDITTEXT_H_
#define _CONTROL_ZKEDITTEXT_H_

#include "ZKTextView.h"
#include "ime/IMEContext.h"

class ZKEditText : public ZKTextView {
public:
	ZKEditText(HWND hParentWnd);
	virtual ~ZKEditText();

	void setPassword(bool isPassword);
	bool isPassword() const { return mIMETextInfo.isPassword; }

protected:
	virtual void onBeforeCreateWindow(const Json::Value &json);
	virtual const char* getClassName() const { return ZK_EDITTEXT; }

	virtual void onDraw(HDC hdc);
	virtual BOOL onTouchEvent(const MotionEvent &ev);

	void drawEditText(HDC hdc);

private:
	void parseEditTextAttributeFromJson(const Json::Value &json);

	class IMETextUpdateListener : public IMEContext::IIMETextUpdateListener {
	public:
		IMETextUpdateListener(ZKEditText *pEditText) : mEditTextPtr(pEditText) { }
		virtual void onIMETextUpdate(const std::string &text) {
			mEditTextPtr->setText(text);
		}

	private:
		ZKEditText *mEditTextPtr;
	};

private:
	IMETextUpdateListener mIMETextUpdateListener;
	IMEContext::SIMETextInfo mIMETextInfo;

	string mHintText;
};

#endif /* _CONTROL_ZKEDITTEXT_H_ */
