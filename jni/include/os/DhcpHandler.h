#ifndef _OS_DHCP_HANDLER_H_
#define _OS_DHCP_HANDLER_H_

#include <pthread.h>

class DhcpHandler {
public:
	DhcpHandler();
	~DhcpHandler();

	int ethernetRequestIp();
	int ethernetReleaseIp();
	
	int wifiRequestIp();
	int wifiReleaseIp();

private:
	pthread_mutex_t mMutex;
};

#endif	//_OS_DHCP_HANDLER_H_
