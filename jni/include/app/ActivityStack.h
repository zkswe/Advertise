/*
 * ActivityStack.h
 *
 *  Created on: Aug 21, 2017
 *      Author: guoxs
 */

#ifndef _APP_ACTIVITY_STACK_H_
#define _APP_ACTIVITY_STACK_H_

#include "Activity.h"

class ActivityStack {
public:
	typedef std::pair<std::string, Activity*> ActivityEntry;
	typedef std::vector<ActivityEntry> ActivityCollection;

	static ActivityStack* getInstance();

	Activity* top() const;
	Activity* getHomeActivity() const;

	bool isHomeActivity(const char *appName) const;

	const char* currentAppName() const;

	bool push(const char *appName, Intent *intentPtr = NULL);
	bool pop();

	bool goBack();
	bool goHome();

	int depth() const { return mActivityCollection.size(); }

	void switchTo(const char *appName, Intent *intentPtr = NULL);
	void clearStack();

	void dumpStack() const;

private:
	ActivityStack();
	virtual ~ActivityStack();

	Activity* innerPush(const char *appName, Intent *intentPtr);
	bool innerPop();

	int popTo(Activity *which, Intent *intentPtr = NULL);
	Activity* underTop() const;

	bool popToHome(Intent *intentPtr = NULL);

	void doSwitchEffect(Activity *prev, Activity *next, bool switchTo = true);
	Activity* searchActivityByName(std::string appName) const;

private:
	ActivityCollection mActivityCollection;
};

#define ACTIVITYSTACK			ActivityStack::getInstance()

#endif /* _APP_ACTIVITY_STACK_H_ */
