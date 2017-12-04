/*
 * ZKWindow.h
 *
 *  Created on: Jun 10, 2017
 *      Author: guoxs
 */

#ifndef _WINDOW_ZKWINDOW_H_
#define _WINDOW_ZKWINDOW_H_

#include <vector>
#include "control/ZKBase.h"

class ZKWindow : public ZKBase {
public:
	ZKWindow(HWND hParentWnd);
	virtual ~ZKWindow();

	BOOL isModalWindow() const { return mIsModal; }

	virtual ZKBase* findControlByID(int id);
	virtual void getAllControls(vector<ZKBase*> &controlsList);

	void showWnd();
	void hideWnd();
	bool isWndShow() const;

protected:
	virtual void onBeforeCreateWindow(const Json::Value &json);
	virtual void onAfterCreateWindow(const Json::Value &json);
	virtual const char* getClassName() const { return ZK_WINDOW; }

	virtual BOOL onInterceptMessage(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
	virtual BOOL onTouchEvent(const MotionEvent &ev);
	virtual void onTimer(int id);

	void clearControlsList();

	BOOL isWindowClass(const char *pClassName) const;
	BOOL checkHideModalWindow(int x, int y);

private:
	void _section_(zk) parseWindowAttributeFromJson(const Json::Value &json);
	void _section_(zk) parseControlsFromJson(const Json::Value &json);

protected:
	vector<ZKBase*> mControlsList;
	BOOL mIsModal;

	int mHideTimeOut;	// 模式窗口超时隐藏
};

#endif /* _WINDOW_ZKWINDOW_H_ */
