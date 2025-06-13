#ifndef MEMORYDBCONNECTION_HPP
#define MEMORYDBCONNECTION_HPP

#include <vector>
#include "WalletDTO.hpp"
#include "MovementDTO.hpp"
#include "OracleDTO.hpp"

using namespace std;

class MemoryDBConnection
    {
    private:
        vector<WalletDTO*> walletsDB;
        vector<MovementDTO*> movementsDB;
        map<string,OracleDTO*> oracleDB;

    public:
        MemoryDBConnection();
        virtual ~MemoryDBConnection();

        vector<WalletDTO*>& getWalletList();
        vector<MovementDTO*>& getMovementList();
        map<string,OracleDTO*>& getOracleMap();
        void close();
    };

#endif
