#ifndef MOVEMENTDBDAO_HPP
#define MOVEMENTDBDAO_HPP

#include <string>
#include "AbstractMovementDAO.hpp"
#include "ServerDBConnection.hpp"

class MovementDBDAO final : public AbstractMovementDAO
    {
    private:
        static const string SQL_registerTransaction;
        static const string SQL_getHistory;

        ServerDBConnection *serverDBConnection;

    public:
        MovementDBDAO(ServerDBConnection *serverDBConnection);
        virtual ~MovementDBDAO();

        virtual void registerTransaction(MovementDTO *movement);
        virtual vector<MovementDTO*> getHistoryByWalletId(int walletId);
};

#endif
