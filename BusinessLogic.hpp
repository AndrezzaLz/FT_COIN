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
#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <algorithm>

class BusinessLogic {
private:
    AbstractWalletDAO* walletDAO;
    AbstractMovementDAO* movementDAO;
    AbstractOracleDAO* oracleDAO;

public:
    BusinessLogic(AbstractWalletDAO* wDAO, AbstractMovementDAO* mDAO, AbstractOracleDAO* oDAO)
        : walletDAO(wDAO), movementDAO(mDAO), oracleDAO(oDAO) {}

    // Calculate wallet balance based on movements and quotes
    double calculateWalletBalance(int walletId) {
        std::vector<MovementDTO*> movements = movementDAO->getHistoryByWalletId(walletId);
        double balance = 0.0;

        for (const MovementDTO* mov : movements) {
            OracleDTO* quote = oracleDAO->getQuoteByDate(mov->getDate());
            double value = mov->getQuantity() * quote->getQuote();

            if (mov->getOperationType() == 'C') {
                balance -= value;
            } else if (mov->getOperationType() == 'V') {
                balance += value;
            }

            delete quote;  // liberar memória
        }

        return balance;
    }

    // Calculate gain/loss of the wallet considering current value of coins
    double calculateGainLoss(int walletId) {
        std::vector<MovementDTO*> movements = movementDAO->getHistoryByWalletId(walletId);
        if (movements.empty()) return 0.0;

        double totalCost = 0.0;
        double currentValue = 0.0;
        Date latestDate;

        for (const MovementDTO* mov : movements) {
            OracleDTO* quote = oracleDAO->getQuoteByDate(mov->getDate());
            double value = mov->getQuantity() * quote->getQuote();

            if (mov->getOperationType() == 'C') {
                totalCost += value;
            } else if (mov->getOperationType() == 'V') {
                totalCost -= value;
            }

            if (mov->getDate() > latestDate) {
                latestDate = mov->getDate();
            }

            delete quote;  // liberar memória
        }

        OracleDTO* currentQuote = oracleDAO->getQuoteByDate(latestDate);
        for (const MovementDTO* mov : movements) {
            currentValue += mov->getQuantity() * currentQuote->getQuote();
        }
        delete currentQuote;

        return currentValue - totalCost;
    }

    // Simple report: list wallets by ID and holder name, sorted by ID
    void reportWallets() {
        std::vector<WalletDTO*> wallets = walletDAO->getAllWallets();

        std::sort(wallets.begin(), wallets.end(), [](WalletDTO* a, WalletDTO* b) {
            return a->getWalletId() < b->getWalletId();
        });

        std::cout << "ID\tHolder\n";
        for (const WalletDTO* w : wallets) {
            std::cout << w->getWalletId() << "\t" << w->getHolderName() << "\n";
        }
    }

    // Detailed report: balance and transaction history of a wallet
    void detailedWalletReport(int walletId) {
        WalletDTO* wallet = walletDAO->getWalletById(walletId);
        std::vector<MovementDTO*> movements = movementDAO->getHistoryByWalletId(walletId);

        std::cout << "\nWallet of: " << wallet->getHolderName()
                  << " | Broker: " << wallet->getExchangeName() << "\n";
        std::cout << "Transactions:\n";
        std::cout << "Date\t\tType\tQuantity\tQuote\t\tValue\n";

        for (const MovementDTO* mov : movements) {
            OracleDTO* quote = oracleDAO->getQuoteByDate(mov->getDate());
            double value = mov->getQuantity() * quote->getQuote();

            std::cout << mov->getDate() << "\t"
                      << mov->getOperationType() << "\t"
                      << mov->getQuantity() << "\t\t"
                      << std::fixed << std::setprecision(2) << quote->getQuote() << "\t\t"
                      << value << "\n";

            delete quote;  // liberar memória
        }

        double balance = calculateWalletBalance(walletId);
        std::cout << "Current Balance: $ " << std::fixed << std::setprecision(2) << balance << "\n";
    }
};

#endif