/*
 * EasyUIContext.h
 *
 *  Created on: Aug 24, 2017
 *      Author: guoxs
 */

#ifndef _ENTRY_EASYUI_CONTEXT_H_
#define _ENTRY_EASYUI_CONTEXT_H_

#include <vector>
#include <stddef.h>

#include "control/Common.h"
#include "system/Mutex.h"
#include "ime/IMEContext.h"

class Intent;
class BaseApp;
class IMEBaseApp;

class EasyUIContext {
	friend int main(int argc, const char *argv[]);
public:
	class ITouchListener {
	public:
		virtual ~ITouchListener() { }
		virtual bool onTouchEvent(const MotionEvent &ev) = 0;
	};

	void registerGlobalTouchListener(ITouchListener *pListener);
	void unregisterGlobalTouchListener(ITouchListener *pListener);
	bool notifyGlobalTouchListener(const MotionEvent &ev);

public:
	static EasyUIContext* getInstance();

	// 传入的intentPtr必须是堆里new出来的对象，最后由框架内部完成delete
	void openActivity(const char *appName, Intent *intentPtr = NULL);
	void goBack();
	void goHome();

	const char* currentAppName() const;

	void showStatusBar();
	void hideStatusBar();
	bool isStatusBarShow() const;
	void showNaviBar();
	void hideNaviBar();
	bool isNaviBarShow() const;

	void powerOff();
	void powerOn();
	bool isPowerOff() const;

	void setScreensaverTimeOut(int timeOut);
	void resetScreensaverTimeOut();
	int getScreensaverTimeOut() const;

	void setScreensaverEnable(bool isEnable);
	bool isScreensaverEnable() const;

	void screensaverOn();
	void screensaverOff();
	bool isScreensaverOn() const;

	void showIME(const IMEContext::SIMETextInfo &info, IMEContext::IIMETextUpdateListener *pListener);
	void hideIME();
	void performHideIME();

private:
	EasyUIContext();
	~EasyUIContext();

	bool initEasyUI();
	void deinitEasyUI();
	void runEasyUI();

	bool initLib();
	void deinitLib();

	void loadStatusBar();
	void loadNaviBar();

	void unloadStatusBar();
	void unloadNaviBar();

	void performEasyUIInit();
	void performEasyUIDeinit();
	void performStartupApp();
	void performLoadStatusBar();
	void performLoadNaviBar();
	void performScreensaver();

private:
	std::vector<ITouchListener *> mGlobalTouchListenerList;
	mutable Mutex mTouchLock;

	BaseApp *mStatusBarPtr;
	BaseApp *mNaviBarPtr;
	BaseApp *mScreensaverPtr;

	IMEBaseApp *mIMEPtr;

	struct {
		void (*onEasyUIInit)(EasyUIContext*);
		void (*onEasyUIDeinit)(EasyUIContext*);
		const char* (*onStartupApp)(EasyUIContext*);
	} mLibFunInfo;

	void *mLibHandle;
	void *mInternalLibHandle;

	bool mIsUpgradeMode;

	int mScreensaverTimeOut;
	bool mIsScreensaverEnable;
};

#define EASYUICONTEXT			EasyUIContext::getInstance()

#endif /* _ENTRY_EASYUI_CONTEXT_H_ */
