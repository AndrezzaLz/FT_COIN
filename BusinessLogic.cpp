#include "BusinessLogic.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>

BusinessLogic::BusinessLogic(AbstractWalletDAO* wDAO, AbstractMovementDAO* mDAO, AbstractOracleDAO* oDAO)
    : walletDAO(wDAO), movementDAO(mDAO), oracleDAO(oDAO) {}

double BusinessLogic::calculateWalletBalance(int walletId) {
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

        delete quote;
    }

    return balance;
}

double BusinessLogic::calculateGainLoss(int walletId) {
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

        delete quote;
    }

    OracleDTO* currentQuote = oracleDAO->getQuoteByDate(latestDate);
    for (const MovementDTO* mov : movements) {
        currentValue += mov->getQuantity() * currentQuote->getQuote();
    }
    delete currentQuote;

    return currentValue - totalCost;
}

void BusinessLogic::reportWallets() {
    std::vector<WalletDTO*> wallets = walletDAO->getAllWallets();

    std::sort(wallets.begin(), wallets.end(), [](WalletDTO* a, WalletDTO* b) {
        return a->getWalletId() < b->getWalletId();
    });

    std::cout << "ID\tTitular\n";
    for (const WalletDTO* w : wallets) {
        std::cout << w->getWalletId() << "\t" << w->getHolderName() << "\n";
    }
}

void BusinessLogic::detailedWalletReport(int walletId) {
    WalletDTO* wallet = walletDAO->getWalletById(walletId);
    std::vector<MovementDTO*> movements = movementDAO->getHistoryByWalletId(walletId);

    std::cout << "\nCarteira de: " << wallet->getHolderName()
              << " | Corretora: " << wallet->getExchangeName() << "\n";
    std::cout << "Movimentações:\n";
    std::cout << "Data\t\tTipo\tQuantidade\tCotação\t\tValor\n";

    for (const MovementDTO* mov : movements) {
        OracleDTO* quote = oracleDAO->getQuoteByDate(mov->getDate());
        double value = mov->getQuantity() * quote->getQuote();

        std::cout << mov->getDate() << "\t"
                  << mov->getOperationType() << "\t"
                  << mov->getQuantity() << "\t\t"
                  << std::fixed << std::setprecision(2) << quote->getQuote() << "\t\t"
                  << value << "\n";

        delete quote;
    }

    double balance = calculateWalletBalance(walletId);
    std::cout << "Saldo Atual: R$ " << std::fixed << std::setprecision(2) << balance << "\n";
}
