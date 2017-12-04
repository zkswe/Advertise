/*
 * ZKCircleBar.h
 *
 *  Created on: Sep 5, 2017
 *      Author: guoxs
 */

#ifndef _CONTROL_ZKCIRCLEBAR_H_
#define _CONTROL_ZKCIRCLEBAR_H_

#include "ZKBase.h"

class ZKCircleBar : public ZKBase {
public:
	ZKCircleBar(HWND hParentWnd);
	virtual ~ZKCircleBar();

	void setMax(int max);
	int getMax() const { return mMax; }

	void setProgress(int progress);
	int getProgress() const { return mProgress; }

protected:
	virtual void onBeforeCreateWindow(const Json::Value &json);
	virtual const char* getClassName() const { return ZK_CIRCLEBAR; }

	virtual void onDraw(HDC hdc);

	void _section_(zk) drawProgress(HDC hdc);
	void _section_(zk) drawProgressText(HDC hdc);

private:
	void _section_(zk) parseCircleBarAttributeFromJson(const Json::Value &json);

protected:
	typedef enum {
		E_TEXT_TYPE_NONE,				// 不绘制文本
		E_TEXT_TYPE_NUM,				// 数字
		E_TEXT_TYPE_NUM_WITH_UNIT		// 数字 + 单位
	} EProgressTextType;

	int mMax;
	int mProgress;

	EProgressTextType mTextType;
	PLOGFONT mLogFontPtr;
	int mTextSize;
	int mTextColor;
	string mUnitStr;		// 单位, 默认: %

	int mStartAngle;
	bool mIsClockwise;	// 顺时针方向

	PBITMAP mProgressPicPtr;
	HDC mProgressDC;
};

#endif /* _CONTROL_ZKCIRCLEBAR_H_ */
