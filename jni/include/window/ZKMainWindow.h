/*
 * ZKMainWindow.h
 *
 *  Created on: Jun 10, 2017
 *      Author: guoxs
 */

#ifndef _WINDOW_ZKMAINWINDOW_H_
#define _WINDOW_ZKMAINWINDOW_H_

#include "ZKWindow.h"
#include "system/Mutex.h"

class ZKMainWindow : public ZKWindow {
public:
	ZKMainWindow(HWND hParentWnd);
	virtual ~ZKMainWindow();

	HDC getWindowForeground();

public:
	class ITimerListener {
	public:
		virtual ~ITimerListener() { }
		virtual bool onTimer(int id) = 0;
	};

	void registerTimerListener(int id, int time, ITimerListener *pListener);
	void unregisterTimerListener(int id, ITimerListener *pListener);
	void resetTimer(int id, int time);

protected:
	virtual void onBeforeCreateWindow(const Json::Value &json);
	virtual BOOL createWindow();
	virtual int procCtrlFun(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);

private:
	const char* getClassName() const { return NULL; }
	void _section_(zk) parseMainWindowAttributeFromJson(const Json::Value &json);

	void notifyTimerListener(int id);

private:
	typedef struct {
		int id;
		ITimerListener *pListener;
	} STimerInfo;
	std::vector<STimerInfo> mTimerInfoList;

	mutable Mutex mTimerLock;
};

#endif /* _WINDOW_ZKMAINWINDOW_H_ */
