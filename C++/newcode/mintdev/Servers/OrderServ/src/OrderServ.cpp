#include <OrderServ.h>
#include <iostream>
#include <odb/pgsql/transaction.hxx>
#include <odb/pgsql/database.hxx>

OrderServ::OrderServ()
{
  InitInstance();
}

bool OrderServ::InitInstance()
{
  if(!connectDb())
  {
    std::cerr << "Database connection failed";
    return false;
  }
}

bool OrderServ::connectDb()
{
  try
  {
    _db = new odb::pgsql::database("vagrant", "vagrant", "test", "localhost", 5432);
    // To test connection
    _db->connection();
    return true;
  }
  catch(const odb::exception& e)
  {
    std::cerr << "Failed to connect to DB: " << e.what();
    return false;
  }
}
