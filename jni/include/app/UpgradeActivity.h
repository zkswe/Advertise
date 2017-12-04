/*
 * UpgradeActivity.h
 *
 *  Created on: Sep 3, 2017
 *      Author: guoxs
 */

#ifndef _APP_UPGRADE_ACTIVITY_H_
#define _APP_UPGRADE_ACTIVITY_H_

#include "Activity.h"
#include "os/UpgradeMonitor.h"
#include "control/ZKListView.h"

class UpgradeActivity : public Activity {
public:
	UpgradeActivity();
	virtual ~UpgradeActivity();

protected:
	virtual const char* getAppName() const;

	virtual void onCreate();
	virtual void onClick(ZKBase *pBase);

private:
	class UpgradeStatusListener : public UpgradeMonitor::IUpgradeStatusListener {
	public:
		UpgradeStatusListener(UpgradeActivity *pActivity) : mActivity(pActivity) { }
		virtual void notify(int what, int status, const char *msg);

	private:
		UpgradeActivity *mActivity;
	};

	class ProcessThread : public Thread {
	public:
		ProcessThread(UpgradeActivity *pActivity) : mActivity(pActivity) { }

	protected:
		virtual bool threadLoop();

	private:
		UpgradeActivity *mActivity;
	};

	class UpgradeInfoListAdapter : public ZKListView::AbsListAdapter {
	public:
		virtual int getCount() const;
		virtual void obtainListItemData(ZKListView::ZKListItem *pListItem, int index);
	};

	class UpgradeInfoItemClickListener : public ZKListView::IItemClickListener {
	public:
		virtual void onItemClick(ZKListView *pListView, int index, int itemID);
	};

private:
	UpgradeStatusListener mUpgradeStatusListener;
	UpgradeInfoListAdapter mUpgradeInfoListAdapter;
	UpgradeInfoItemClickListener mUpgradeInfoItemClickListener;
	ProcessThread mProcessThread;
};

#endif /* _APP_UPGRADE_ACTIVITY_H_ */
