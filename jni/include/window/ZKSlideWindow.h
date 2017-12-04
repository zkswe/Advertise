/*
 * ZKSlideWindow.h
 *
 *  Created on: Jul 3, 2017
 *      Author: guoxs
 */

#ifndef _WINDOW_ZKSLIDEWINDOW_H_
#define _WINDOW_ZKSLIDEWINDOW_H_

#include "ZKWindow.h"
#include "utils/VelocityTracker.h"

class ZKSlideWindow : public ZKWindow {
public:
	ZKSlideWindow(HWND hParentWnd);
	virtual ~ZKSlideWindow();

public:
	class ISlideItemClickListener {
	public:
		virtual ~ISlideItemClickListener() { }
		virtual void onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) = 0;
	};

	void setSlideItemClickListener(ISlideItemClickListener *pListener) {
		mSlideItemClickListenerPtr = pListener;
	}

private:
	typedef struct {
		int curStatus;
		PBITMAP itemStatusPicTab[S_CONTROL_STATUS_TAB_LEN];
		int itemStatusTextColorTab[S_CONTROL_STATUS_TAB_LEN];
		string text;
	} SSlideItemInfo;

	typedef struct {
		SSlideItemInfo *itemList;
		int size;
	} SSlidePageInfo;

	typedef struct {
		SSlidePageInfo *pageList;
		int size;
	} SSlidePageInfosList;

protected:
	virtual void onBeforeCreateWindow(const Json::Value &json);
	virtual const char* getClassName() const { return ZK_SLIDEWINDOW; }

	virtual void onDraw(HDC hdc);
	virtual BOOL onTouchEvent(const MotionEvent &ev);
	virtual void onTimer(int id);

	void drawBackgroundBlt(HDC hdc);
	void _section_(zk) drawItems(HDC hdc);
	void _section_(zk) drawItem(HDC hdc, const SSlideItemInfo *pItem, int xOffset, int yOffset);

private:
	BOOL isSliding() const { return mIsSliding; }
	BOOL needToRoll() const { return (mFirstPageXOffset % mPosition.mWidth != 0); }

	BOOL performRollAnimation();		// 执行滚动动画
	BOOL wantToSlideToNextPage();
	BOOL wantToSlideToPrevPage();

	int hitItemIndex(int x, int y) const;

	const PBITMAP getItemCurStatusPic(const SSlideItemInfo *pItem) const;
	int getItemCurTextColor(const SSlideItemInfo *pItem) const;

	void _section_(zk) parseSlideWindowAttributeFromJson(const Json::Value &json);

private:
	UINT mRows;
	UINT mCols;

	SIZE mItemSize;
	SIZE mIconSize;

	PLOGFONT mLogFontPtr;
	int mFontSize;

	LayoutPadding mPadding;

	int mCurrentPage;
	int mFirstPageXOffset;
	int mFirstPageXOffsetOfDown;

	int mHitIndex;

	BOOL mIsSliding;
	BOOL mIsDamping;

	UINT mRollSpeed;

	VelocityTracker mVelocityTracker;
	BOOL mNeedToTurnPage;

	SSlidePageInfosList mSlidePageInfosList;

	ISlideItemClickListener *mSlideItemClickListenerPtr;

	HDC mBackgroundDC;
};

#endif /* _WINDOW_ZKSLIDEWINDOW_H_ */
