#include <ClientPortDaemon.h>

void ClientPortDaemon::handleAccept(int fd, short ev)
{
}

int ClientPortDaemon::listen(int portnu)
{
  return MSocket::Create(portnu, 1, true);
}

ClientPortDaemon::ClientPortDaemon()
{
}

ClientPortDaemon::~ClientPortDaemon()
{
}

