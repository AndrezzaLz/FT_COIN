#include <mariadb/conncpp.hpp>

#ifndef SERVERDBCONNECTION_H
#define SERVERDBCONNECTION_H

using namespace std;

class ServerDBConnection final
    {
  private:
    string serverPort = "3306";
    string serverIP = "143.106.243.64";
    string serverDatabase = "PooI_25_B09"; 
    string serverUser = "PooI_25_B09;
    string serverPassword = "YEdm2DlaRP";

    sql::Driver *driver; = NULL;
    sql::Connection *connection;

   public:
     ServerDBConnection();
     virtual ~ServerDBConnection();
     sql::Connection *getConnection() const;
};

#endif //SERVERDBCONNECTION_H
