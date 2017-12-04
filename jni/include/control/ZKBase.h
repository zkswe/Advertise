/*
 * ZKBase.h
 *
 *  Created on: Jun 8, 2017
 *      Author: guoxs
 */

#ifndef _CONTROL_ZKBASE_H_
#define _CONTROL_ZKBASE_H_

#include <string>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#include <mgplus/mgplus.h>
#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>

#include "Common.h"
#include "json/json.h"

using namespace std;

class ZKBase {
	friend class ControlFactory;
public:
	ZKBase(HWND hParentWnd);
	virtual ~ZKBase();

	BOOL create(const Json::Value &json);

	HWND getParentWnd() const { return mParentWnd; }
	HWND getWnd() const { return mWnd; }
	int getID() const { return mID; }

	void setPosition(const LayoutPosition &position);
	const LayoutPosition& getPosition() const { return mPosition; }

	void setInvalid(BOOL isInvalid);
	BOOL isInvalid() const;

	void setVisible(BOOL isVisible);
	BOOL isVisible() const;

	void setSelected(BOOL isSelected);
	BOOL isSelected() const;

	void setPressed(BOOL isPressed);
	BOOL isPressed() const;

	void setTouchable(BOOL isTouchable);
	BOOL isTouchable() const;

	void setAnimatable(BOOL isAnimatable);
	BOOL isAnimatable() const;

	BOOL isWndValid() const { return mWnd != HWND_INVALID; }

	void setBackgroundPic(const char *pPicPath);

	virtual const char* getClassName() const { return NULL; }

public:
	class IClickListener {
	public:
		virtual ~IClickListener() { }
		virtual void onClick(ZKBase *pBase) = 0;
	};

	class ILongClickListener {
	public:
		virtual ~ILongClickListener() { }
		virtual void onLongClick(ZKBase *pBase) = 0;
	};

	void setClickListener(IClickListener *pListener) { mClickListenerPtr = pListener; }
	void setLongClickListener(ILongClickListener *pListener) { mLongClickListenerPtr = pListener; }

protected:
	virtual BOOL createWindow();
	virtual void onBeforeCreateWindow(const Json::Value &json);
	virtual void onAfterCreateWindow(const Json::Value &json);

	virtual int procCtrlFun(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
	static int ctrlFun(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);

	virtual BOOL onInterceptMessage(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);
	virtual void onDraw(HDC hdc);
	virtual BOOL onTouchEvent(const MotionEvent &ev);
	virtual void onTimer(int id);

	void _section_(zk) invalidate(const RECT *pDirty = NULL, BOOL isEraseBG = TRUE);

	void _section_(zk) doDraw();

	void _section_(zk) startTimer(int id, int time);
	void _section_(zk) stopTimer(int id);
	void _section_(zk) resetTimer(int id, int time);
	BOOL _section_(zk) hasTimer(int id);

	void _section_(zk) sendMessage(int message, WPARAM wParam, LPARAM lParam);
	void _section_(zk) sendNotifyMessage(int message, WPARAM wParam, LPARAM lParam);

	void _section_(zk) drawBackground(HDC hdc);

private:
	void _section_(zk) parseBaseAttributeFromJson(const Json::Value &json);

protected:
	typedef enum {
		E_CONTROL_STATUS_NORMAL 	= 0x00,
		E_CONTROL_STATUS_PRESSED 	= 0x01,
		E_CONTROL_STATUS_SELECTED	= 0x02,
		E_CONTROL_STATUS_INVALID	= 0x04,
		E_CONTROL_STATUS_VISIBLE	= 0x08
	} EControlStatus;

	const static int S_CONTROL_STATUS_TAB_LEN = 5;

	typedef enum {
		E_CONTROL_FLAG_NONE 		= 0x00,
		E_CONTROL_FLAG_TOUCHABLE 	= 0x01,
		E_CONTROL_FLAG_SWITCH	 	= 0x02,
		E_CONTROL_FLAG_ANIMATABLE	= 0x04
	} EControlFlag;

	typedef enum {
		E_ORIENTATION_HORIZONTAL,
		E_ORIENTATION_VERTICAL
	} EOrientation;

protected:
	HWND mParentWnd;
	HWND mWnd;
	int mID;

	DWORD mDwExStyle;

	int mBackgroundColor;
	PBITMAP mBackgroundPicPtr;

	LayoutPosition mPosition;

	int mControlStatus;
	int mControlFlags;

	IClickListener *mClickListenerPtr;
	ILongClickListener *mLongClickListenerPtr;

	POINT mDownPoint;
	POINT mUpPoint;
	BOOL mHasDown;

	BOOL mIsFirstDraw;
};


#include <vector>

class ControlFactory {
public:
    static ControlFactory* getInstance();

    bool addControlInfo(const char *name, ZKBase* (*create)(HWND hParentWnd));

    void registerControl();
    void unregisterControl();

    ZKBase* create(const char *name, HWND hParentWnd);

private:
    ControlFactory() { }

private:
    typedef struct {
        std::string ctrlName;
        ZKBase* (*create)(HWND hParentWnd);
    } SControlInfo;

    std::vector<SControlInfo> mControlInfos;
};

#define CONTROLFACTORY		ControlFactory::getInstance()
#define REGISTER_CONTROL(name, _ctrl) \
    static struct _ControlFactory_##_ctrl { \
        static ZKBase* create(HWND hParentWnd) { \
            return new _ctrl(hParentWnd); \
        } \
        _ControlFactory_##_ctrl() { \
        	CONTROLFACTORY->addControlInfo(name, create); \
        } \
    } _autoRegister_Control_##_ctrl

#endif /* _CONTROL_ZKBASE_H_ */
