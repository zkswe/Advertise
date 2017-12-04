/*
 * ZKListView.h
 *
 *  Created on: Jun 28, 2017
 *      Author: guoxs
 */

#ifndef _CONTROL_ZKLISTVIEW_H_
#define _CONTROL_ZKLISTVIEW_H_

#include "ZKButton.h"
#include "utils/VelocityTracker.h"

class ZKListView : public ZKBase {
public:
	ZKListView(HWND hParentWnd);
	virtual ~ZKListView();

public:
	class ZKListItem;
	class ZKListSubItem : public ZKButton {
		friend class ZKListItem;
	public:
		ZKListSubItem();
		virtual ~ZKListSubItem();

	protected:
		virtual BOOL createWindow() { return FALSE; }
		virtual const char* getClassName() const { return NULL; }

		void drawSubItem(HDC hdc, int xOffset, int yOffset);
	};

	class ZKListItem : public ZKListSubItem {
		friend class ZKListView;
	public:
		ZKListItem();
		virtual ~ZKListItem();

		// 通过ID值获取子项
		ZKListSubItem* findSubItemByID(int id) const;
		int getSubItemCount() const { return mSubItemCount; }

	protected:
		virtual void onBeforeCreateWindow(const Json::Value &json);

		int hitItemID(const MotionEvent &ev);
		void drawItem(HDC hdc, int xOffset, int yOffset);
		void setItemPressed(int itemID, BOOL isPressed);

	private:
		void parseListItemAttributeFromJson(const Json::Value &json);

	private:
		const static int S_MAX_SUB_ITEM_COUNT = 5;	// 列表项最多包含5个子项
		ZKListSubItem *mListSubItems[S_MAX_SUB_ITEM_COUNT];
		int mSubItemCount;
	};

	/**
	 * 数据与UI绑定适配器
	 */
	class AbsListAdapter {
		friend class ZKListView;
	public:
		virtual ~AbsListAdapter() { }
		virtual int getListItemCount(const ZKListView *pListView) const = 0;
		virtual void obtainListItemData(ZKListView *pListView, ZKListItem *pListItem, int index) = 0;

		/**
		 * 数据更新，重绘UI
		 */
		void notifyDataSetChanged(ZKListView *pListView) {
			if (pListView) {
				pListView->invalidate();
			}
		}
	};

	void setListAdapter(AbsListAdapter *pAdapter) {
		mAdapterPtr = pAdapter;
	}

	class IItemClickListener {
	public:
		virtual ~IItemClickListener() { }
		virtual void onItemClick(ZKListView *pListView, int index, int itemID) = 0;
	};

	class IItemLongClickListener {
	public:
		virtual ~IItemLongClickListener() { }
		virtual void onItemLongClick(ZKListView *pListView, int index, int itemID) = 0;
	};

	void setItemClickListener(IItemClickListener *pListener) { mItemClickListenerPtr = pListener; }
	void setItemLongClickListener(IItemLongClickListener *pListener) { mItemLongClickListenerPtr = pListener; }

	void refreshListView();
	void setSelection(int index);	// 跳转到指定行或列
	int getListItemCount() const;

protected:
	virtual void onBeforeCreateWindow(const Json::Value &json);
	virtual const char* getClassName() const { return ZK_LISTVIEW; }

	virtual void onDraw(HDC hdc);
	virtual BOOL onTouchEvent(const MotionEvent &ev);
	virtual void onTimer(int id);

	void _section_(zk) drawBackgroundGraphics(HDC hdc);
	void _section_(zk) drawListView(HDC hdc);
	void _section_(zk) drawListItem(HDC hdc, int index, int xOffset, int yOffset);

private:
	void _section_(zk) parseListViewAttributeFromJson(const Json::Value &json);
	int getFirstVisibleItemIndex() const;
	void getFirstVisibleItemOffset(int &xOffset, int &yOffset) const;
	int getEndEdgeOffset() const;

	void resetOffset();
	void resetItemPressInfo();

	// 启动回滚
	void startRollback();

	// 停止滚动
	void stopRoll();

	// 循环列表 调整首项偏移值
	void cycleFirstItemOffset();

	/**
	 * 记录列表项按下信息
	 */
	typedef struct {
		BOOL isPressed;
		int index;
		int itemID;
	} SItemPressInfo;

	typedef enum {
		E_EDGE_EFFECT_NONE,
		E_EDGE_EFFECT_DRAG,			// 拖拽效果
		E_EDGE_EFFECT_FADING		// 阴影逐渐褪去效果
	} EEdgeEffect;

	BOOL isVerticalOrientation() const { return mOrientation == E_ORIENTATION_VERTICAL; }
	BOOL isDragEffect() const { return mEdgeEffect == E_EDGE_EFFECT_DRAG; }
	BOOL isFadingEffect() const { return mEdgeEffect == E_EDGE_EFFECT_FADING; }

private:
	UINT mRows;
	UINT mCols;

	int mFirstItemXOffset;
	int mFirstItemYOffset;

	int mFirstItemXOffsetOfDown;	// 记录按下时首项的偏移位置
	int mFirstItemYOffsetOfDown;

	UINT mItemWidth;
	UINT mItemHeight;

	BOOL mIsDamping;
	BOOL mIsRolling;

	BOOL mIsAutoRollback;		// 自动回滚对齐标志
	BOOL mIsCycleEnable;		// 循环列表

	VelocityTracker mVelocityTracker;
	float mCurVelocity;

	EOrientation mOrientation;	// 方向: 0 横向, 1 纵向
	EEdgeEffect mEdgeEffect;

	UINT mDragMaxDistance;

	IItemClickListener *mItemClickListenerPtr;
	IItemLongClickListener *mItemLongClickListenerPtr;

	SItemPressInfo mItemPressInfo;

	ZKListItem *mListItemPtr;
	AbsListAdapter *mAdapterPtr;

	HGRAPHICS mBackgroundGraphics;
};

#endif /* _CONTROL_ZKLISTVIEW_H_ */
