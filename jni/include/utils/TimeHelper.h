/*
 * TimeHelper.h
 *
 *  Created on: May 22, 2017
 *      Author: guoxs
 */

#ifndef _UTILS_TIME_HELPER_H_
#define _UTILS_TIME_HELPER_H_

#include <time.h>

class TimeHelper {
public:
	static long getCurrentTime();

	static struct tm * getDateTime();

	static bool setDateTime(struct tm *ptm);

	/* date str format: 2017-09-13 16:00:00 */
	static bool setDateTime(const char *pDate);
};

#endif /* _UTILS_TIME_HELPER_H_ */
