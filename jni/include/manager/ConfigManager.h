/*
 * ConfigManager.h
 *
 *  Created on: Jun 17, 2017
 *      Author: guoxs
 */

#ifndef _MANAGER_CONFIG_MANAGER_H_
#define _MANAGER_CONFIG_MANAGER_H_

#include <string>

using namespace std;

class ConfigManager {
public:
	static ConfigManager* getInstance();

	string getResFilePath(const string &resFileName);
	int getLongClickTimeOut() const { return mLongClickTimeOut; }

	int getMinBrightness() const { return mMinBrightness; }
	int getMaxBrightness() const { return mMaxBrightness; }

	const string& getStartupLibPath() const { return mStartupLibPath; }

	int getScreensaverTimeOut() const { return mScreensaverTimeOut; }

	const string& getUartName() const { return mUartName; }
	unsigned int getUartBaudRate() const { return mUartBaudRate; }

private:
	ConfigManager();
	void initEasyUICfg();

private:
	string mResPath;
	int mLongClickTimeOut;

	int mMinBrightness;
	int mMaxBrightness;

	string mStartupLibPath;

	int mScreensaverTimeOut;

	string mUartName;
	unsigned int mUartBaudRate;
};

#define CONFIGMANAGER			ConfigManager::getInstance()

#endif /* _MANAGER_CONFIG_MANAGER_H_ */
