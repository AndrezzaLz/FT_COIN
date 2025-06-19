#ifdef USE_MARIADB

#include <iostream>
#include <string>
#include "ServerDBConnection.hpp"

using std::string;

ServerDBConnection::ServerDBConnection()
    {
    try
        {
        this->driver = sql::mariadb::get_driver_instance();
        string connString = "jdbc:mariadb://" + serverIP + ":" + serverPort + "/" + serverDatabase;
        sql::SQLString url(connString);
        sql::Properties properties(
            {
                { "user", this->serverUser },
                { "password", this->serverPassword } });

        this->connection = driver->connect(url, properties);
        }
    catch (sql::SQLException &myException)
        {
        std::cerr << "Erro ao conectar a Plataforma MariaDB: \n" << myException.what() << std::endl;
        exit(1);
        }
    std::cout << "Conectado a 'MariaDB' RDBMS em " + serverIP + ":" + serverPort + "/" + serverDatabase << std::endl;
    }

ServerDBConnection::~ServerDBConnection()
    {
    this->connection->close();
    }

sql::Connection* ServerDBConnection::getConnection() const
    {
    return (this->connection);
    }

#endif
