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

    for (auto const& [key, value] : oracleDB)
        {
        delete value;
        }
    oracleDB.clear();
    }