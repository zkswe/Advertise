/*
 * Common.h
 *
 *  Created on: Jun 7, 2017
 *      Author: guoxs
 */

#ifndef _CONTROL_COMMON_H_
#define _CONTROL_COMMON_H_

#include <unistd.h>

#ifdef USE_ENCRYPT
#define _section_(S) __attribute__ ((__section__(".text"#S)))
#else
#define _section_(S)
#endif

/********************自定义控件名********************/
#define ZK_WINDOW		"zk_window"
#define ZK_SLIDEWINDOW	"zk_slidewindow"
#define ZK_TEXTVIEW		"zk_textview"
#define ZK_BUTTON		"zk_button"
#define ZK_CHECKBOX		"zk_checkbox"
#define ZK_RADIOGROUP	"zk_radiogroup"
#define ZK_SEEKBAR		"zk_seekbar"
#define ZK_CIRCLEBAR	"zk_circlebar"
#define ZK_LISTVIEW		"zk_listview"
#define ZK_POINTER		"zk_pointer"
#define ZK_DIAGRAM		"zk_diagram"
#define ZK_DIGITALCLOCK	"zk_digitalclock"
#define ZK_QRCODE		"zk_qrcode"
#define ZK_EDITTEXT		"zk_edittext"
#define ZK_VIDEOVIEW	"zk_videoview"
/**************************************************/

#define TOUCH_SLOP		10

/********************Timer id**********************/
#define ID_TIMER_FIRST						100
#define ID_LONG_CLICK_TIME_OUT_TIMER		(ID_TIMER_FIRST)
#define ID_SLIDEWINDOW_ROLL_TIMER			(ID_TIMER_FIRST+1)
#define ID_ROTATE_POINTER_TIMER				(ID_TIMER_FIRST+2)
#define ID_LISTVIEW_ROLL_TIMER				(ID_TIMER_FIRST+3)
#define ID_LISTVIEW_ROLLBACK_TIMER			(ID_TIMER_FIRST+4)
#define ID_CLOCK_TIMER						(ID_TIMER_FIRST+5)
#define ID_HIDE_WND_TIME_OUT_TIMER			(ID_TIMER_FIRST+6)
#define ID_ROLL_TEXT_TIMER					(ID_TIMER_FIRST+7)
#define ID_TIMER_LAST						200
/**************************************************/

class MotionEvent {
public:
	typedef enum {
		E_ACTION_NONE,
		E_ACTION_DOWN,
		E_ACTION_UP,
		E_ACTION_MOVE,
		E_ACTION_CANCEL
	} EActionStatus;

	MotionEvent() {
		reset();
	}

	MotionEvent(EActionStatus actionStatus, int x, int y, long eventTime) :
		mActionStatus(actionStatus), mX(x), mY(y), mEventTime(eventTime) {

	}

	MotionEvent(const MotionEvent &ev) :
		mActionStatus(ev.mActionStatus), mX(ev.mX), mY(ev.mY), mEventTime(ev.mEventTime) {

	}

	void reset() {
		mActionStatus = E_ACTION_NONE;
		mX = 0;
		mY = 0;
		mEventTime = 0;
	}

	EActionStatus mActionStatus;
	int mX;
	int mY;
	long mEventTime;
};

class LayoutPosition {
public:
	LayoutPosition(int l = 0, int t = 0, int w = 0, int h = 0) :
		mLeft(l), mTop(t), mWidth(w), mHeight(h) {

	}

	LayoutPosition(const LayoutPosition& lp) :
		mLeft(lp.mLeft), mTop(lp.mTop), mWidth(lp.mWidth), mHeight(lp.mHeight) {

	}

	void offsetPosition(int xOffset, int yOffset) {
		mLeft += xOffset;
		mTop += yOffset;
	}

	bool operator==(const LayoutPosition &lp) const {
		return (mLeft == lp.mLeft) && (mTop == lp.mTop) &&
				(mWidth == lp.mWidth) && (mHeight == lp.mHeight);
	}

	bool operator!=(const LayoutPosition &lp) const {
		return !(*this == lp);
	}

	bool isHit(int x, int y) const {
		return (x >= mLeft) && (x < mLeft + mWidth) &&
				(y >= mTop) && (y < mTop + mHeight);
	}

	int mLeft;
	int mTop;
	int mWidth;
	int mHeight;
};

class LayoutPadding {
public:
	LayoutPadding(int pl = 0, int pt = 0, int pr = 0, int pb = 0) :
		mPaddingLeft(pl), mPaddingTop(pt),
		mPaddingRight(pr), mPaddingBottom(pb) {

	}

	LayoutPadding(const LayoutPadding& lp) :
		mPaddingLeft(lp.mPaddingLeft), mPaddingTop(lp.mPaddingTop),
		mPaddingRight(lp.mPaddingRight), mPaddingBottom(lp.mPaddingBottom) {

	}

	int mPaddingLeft;
	int mPaddingTop;
	int mPaddingRight;
	int mPaddingBottom;
};

#endif /* _CONTROL_COMMON_H_ */
