#ifndef _NET_DHCP_HANDLER_H_
#define _NET_DHCP_HANDLER_H_

#include "system/Mutex.h"

class DhcpHandler {
public:
	DhcpHandler();
	~DhcpHandler();
	
	int ethernetRequestIp();
	int ethernetReleaseIp();
	int wifiRequestIp();
	int wifiReleaseIp();
	
//	int getIp(char *netName, char *ipstr);
	
private:
	mutable Mutex mLock;
};

#endif	//_NET_DHCP_HANDLER_H_
