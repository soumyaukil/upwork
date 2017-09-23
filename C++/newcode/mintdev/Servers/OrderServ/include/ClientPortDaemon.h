#ifndef CLIENTPORTDAEMON_H
#define CLIENTPORTDAEMON_H

#include "MSocket/MSocket.h"

class ClientPortDaemon : public MSocket  
{
	public:
	void handleAccept(int fd, short ev);
	int listen(int portnu);
	ClientPortDaemon();
	virtual ~ClientPortDaemon();
};

#endif
