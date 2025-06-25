#ifndef BUSINESS_LOGIC_HPP
#define BUSINESS_LOGIC_HPP

#include "AbstractWalletDAO.hpp"
#include "AbstractMovementDAO.hpp"
#include "AbstractOracleDAO.hpp"
#include "WalletDTO.hpp"
#include "MovementDTO.hpp"
#include "OracleDTO.hpp"
#include "Date.hpp"

#include <vector>
#include <string>

class BusinessLogic
{
private:
    AbstractWalletDAO *walletDAO;
    AbstractMovementDAO *movementDAO;
    AbstractOracleDAO *oracleDAO;

public:
    BusinessLogic(AbstractWalletDAO *wDAO, AbstractMovementDAO *mDAO, AbstractOracleDAO *oDAO);

    double calculateWalletBalance(int walletId);
    double calculateGainLoss(int walletId);
    void reportWallets();
    void detailedWalletReport(int walletId);
};

#endif
