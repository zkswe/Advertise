/*
 * BrightnessHelper.h
 *
 *  Created on: Sep 23, 2017
 *      Author: guoxs
 */

#ifndef _UTILS_BRIGHTNESS_HELPER_H_
#define _UTILS_BRIGHTNESS_HELPER_H_

class BrightnessHelper {
public:
	int getMaxBrightness() const;
	int getBrightness() const;	
	void setBrightness(int brightness);

	void screenOff();
	void screenOn();
	bool isScreenOn() const;

	static BrightnessHelper* getInstance();

private:
	BrightnessHelper();

private:
	int mMinBrightness;
	int mMaxBrightness;
	int mBrightness;

	float mRatio;

	bool mIsScreenOn;
};

#define BRIGHTNESSHELPER		BrightnessHelper::getInstance()

#endif /* _UTILS_BRIGHTNESS_HELPER_H_ */
