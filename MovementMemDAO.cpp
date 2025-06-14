#include "MovementMemDAO.hpp"
#include "MemoryDBConnection.hpp"
#include "MovementDTO.hpp"
#include <algorithm>

int MovementMemDAO::lastMovementId = 0;

MovementMemDAO::MovementMemDAO(MemoryDBConnection *memoryDBConnection) : memoryDBConnection(memoryDBConnection)
    {
    }

MovementMemDAO::~MovementMemDAO()
    {
    }

void MovementMemDAO::registerTransaction(MovementDTO* movement)
    {
        movement->setMovementId(++lastMovementId);
        memoryDBConnection->getMovementList().push_back(movement);    
    }

vector<MovementDTO*> MovementMemDAO::getHistoryByWalletId(int walletId)
    {
    vector<MovementDTO*> history;    
    for(MovementDTO* movement : memoryDBConnection->getMovementList())
        {
        if (movement->getWalletId() == walletId)
            {
            history.push_back(movement);    
            }    
        }
        
    sort(history.begin(), history.end(), [](MovementDTO* a, MovementDTO* b) 
        {
    return a->getDate() < b->getDate(); 
        });
    return (history);
    }
 