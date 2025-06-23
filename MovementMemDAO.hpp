#ifndef MOVEMENTMEMDAO_HPP
#define MOVEMENTMEMDAO_HPP

#include "AbstractMovementDAO.hpp"
#include "MemoryDBConnection.hpp"

class MovementMemDAO final : public AbstractMovementDAO
{
private:
    MemoryDBConnection *memoryDBConnection;
    static int lastMovementId;

public:
    MovementMemDAO(MemoryDBConnection *memoryDBConnection);
    virtual ~MovementMemDAO();

    virtual void registerTransaction(MovementDTO *movement);
    virtual vector<MovementDTO *> getHistoryByWalletId(int walletId);
};

#endif
