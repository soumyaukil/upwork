#ifndef ORDERSERV_H
#define ORDERSERV_H

#include <ClientPortDaemon.h>
#include <odb/database.hxx>

class OrderServ
{
  public:
    OrderServ();
    bool CheckDatabase();
    bool connectDb();
    ClientPortDaemon mclientport1; //the client port daemon
    ClientPortDaemon mclientport2; //the client port daemon
    ClientPortDaemon mclientport3; //the client port daemon

    bool InitInstance();
    int ExitInstance();
  private:
    odb::core::database      *_db;
};

#endif
