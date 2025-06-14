#include "MemoryDBConnection.hpp"

MemoryDBConnection::MemoryDBConnection()
    {
    walletsDB = vector<WalletDTO*>();
    movementsDB = vector<MovementDTO*>();
    oracleDB = map<string,OracleDTO*>();
    }

MemoryDBConnection::~MemoryDBConnection()
    {
    close();
    }

vector<WalletDTO*>& MemoryDBConnection::getWalletList()
    {
    return this->walletsDB;
    }

vector<MovementDTO*>& MemoryDBConnection::getMovementList()
    {
    return this->movementsDB;
    }

map<string, OracleDTO*>& MemoryDBConnection::getOracleMap()
    {
    return this->oracleDB;
    }

void MemoryDBConnection::close()
    {
    for (WalletDTO *buffer : walletsDB)
        {
        delete buffer;
        buffer = NULL;
        }
    walletsDB.clear();

    for (MovementDTO *buffer : movementsDB)
        {
        delete buffer;
        buffer = NULL;
        }
    movementsDB.clear();

    for (std::map<std::string, OracleDTO*>::iterator it = oracleDB.begin(); it != oracleDB.end(); ++it) 
        {
        delete it->second; // it->second é o ponteiro para OracleDTO*
        }
    oracleDB.clear();
    }