#ifndef ABSTRACTMOVEMENTDAO_H
#define ABSTRACTMOVEMENTDAO_H

#include <vector>
#include "MovementDTO.hpp"

class AbstractMovementDAO
{
public:
    AbstractMovementDAO();
    virtual ~AbstractMovementDAO();

    virtual void registerTransaction(MovementDTO *movement) = 0;
    virtual vector<MovementDTO *> getHistoryByWalletId(int walletId) = 0;
    virtual void clearAll() = 0;
};

#endif
