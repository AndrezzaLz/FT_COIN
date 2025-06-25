#include "BusinessLogic.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>

BusinessLogic::BusinessLogic(AbstractWalletDAO *wDAO, AbstractMovementDAO *mDAO, AbstractOracleDAO *oDAO)
    : walletDAO(wDAO), movementDAO(mDAO), oracleDAO(oDAO) {}

double BusinessLogic::calculateWalletBalance(int walletId)
{
    std::vector<MovementDTO *> movements = movementDAO->getHistoryByWalletId(walletId);
    double balance = 0.0;

    for (const MovementDTO *mov : movements)
    {
        OracleDTO *quote = oracleDAO->getQuoteByDate(mov->getDate());

        if (quote == nullptr)
        {
            std::cerr << "AVISO: Cotacao nao encontrada para a data " << mov->getDate()
                      << " (ID da Movimentacao: " << mov->getMovementId() << ") na carteira " << walletId
                      << ". Movimentacao ignorada no balanco." << std::endl;
            continue;
        }

        double value = mov->getQuantity() * quote->getQuote();

        if (mov->getOperationType() == OperationType::BUY)
        {
            balance -= value;
        }
        else if (mov->getOperationType() == OperationType::SELL)
        {
            balance += value;
        }

        delete quote;
    }

    for (MovementDTO *mov_ptr : movements)
    {
        delete mov_ptr;
    }

    return balance;
}

double BusinessLogic::calculateGainLoss(int walletId)
{
    std::vector<MovementDTO *> movements = movementDAO->getHistoryByWalletId(walletId);
    if (movements.empty())
        return 0.0;

    double totalCost = 0.0;
    double totalSaleValue = 0.0;
    Date latestDate;

    if (!movements.empty())
    {
        latestDate = movements[0]->getDate();
    }
    else
    {
        latestDate = Date();
    }

    for (const MovementDTO *mov : movements)
    {
        OracleDTO *quote = oracleDAO->getQuoteByDate(mov->getDate());

        if (quote == nullptr)
        {
            std::cerr << "AVISO: Cotacao nao encontrada para a data " << mov->getDate()
                      << " (ID da Movimentacao: " << mov->getMovementId() << ") na carteira " << walletId
                      << ". Movimentacao ignorada nos calculos de ganho/perda." << std::endl;
            continue;
        }

        double valueAtMovementDate = mov->getQuantity() * quote->getQuote();

        if (mov->getOperationType() == OperationType::BUY)
        {
            totalCost += valueAtMovementDate;
        }
        else if (mov->getOperationType() == OperationType::SELL)
        {
            totalCost -= valueAtMovementDate;
        }

        if (mov->getDate() > latestDate)
        {
            latestDate = mov->getDate();
        }

        delete quote;
    }

    double netCoinQuantity = 0.0;
    for (const MovementDTO *mov : movements)
    {
        if (mov->getOperationType() == OperationType::BUY)
        {
            netCoinQuantity += mov->getQuantity();
        }
        else if (mov->getOperationType() == OperationType::SELL)
        {
            netCoinQuantity -= mov->getQuantity();
        }
    }

    OracleDTO *currentQuote = oracleDAO->getQuoteByDate(latestDate);
    double remainingAssetValue = 0.0;
    if (currentQuote == nullptr)
    {
        std::cerr << "AVISO: Cotacao atual nao encontrada para a data " << latestDate
                  << ". O valor dos ativos restantes sera considerado zero no calculo de ganho/perda." << std::endl;
    }
    else
    {
        remainingAssetValue = netCoinQuantity * currentQuote->getQuote();
        delete currentQuote;
    }

    double gainLoss = totalSaleValue + remainingAssetValue - totalCost;

    for (MovementDTO *mov_ptr : movements)
    {
        delete mov_ptr;
    }

    return gainLoss;
}

void BusinessLogic::reportWallets()
{
    std::vector<WalletDTO *> wallets = walletDAO->getAllWallets();

    std::sort(wallets.begin(), wallets.end(), [](WalletDTO *a, WalletDTO *b)
              { return a->getWalletId() < b->getWalletId(); });

    std::cout << "ID\tTitular\n";
    for (const WalletDTO *w : wallets)
    {
        std::cout << w->getWalletId() << "\t" << w->getHolderName() << "\n";
    }

    for (WalletDTO *wallet_ptr : wallets)
    {
        delete wallet_ptr;
    }
}

void BusinessLogic::detailedWalletReport(int walletId)
{
    WalletDTO *wallet = walletDAO->getWalletById(walletId);

    if (wallet == nullptr)
    {
        std::cerr << "ERRO: Carteira com ID " << walletId << " nao encontrada para relatorio detalhado." << std::endl;
        return;
    }

    std::vector<MovementDTO *> movements = movementDAO->getHistoryByWalletId(walletId);

    std::cout << "\nCarteira de: " << wallet->getHolderName()
              << " | Corretora: " << wallet->getExchangeName() << "\n";
    std::cout << "Movimentações:\n";
    std::cout << "Data\t\tTipo\tQuantidade\tCotação\t\tValor\n";

    for (const MovementDTO *mov : movements)
    {
        OracleDTO *quote = oracleDAO->getQuoteByDate(mov->getDate());

        if (quote == nullptr)
        {
            std::cerr << "AVISO: Cotacao nao encontrada para a data " << mov->getDate()
                      << " (ID da Movimentacao: " << mov->getMovementId() << ") na carteira " << walletId
                      << ". Movimento ignorado no relatorio detalhado." << std::endl;
            continue;
        }

        double value = mov->getQuantity() * quote->getQuote();

        std::string operationTypeStr = "";
        if (mov->getOperationType() == OperationType::BUY)
        {
            operationTypeStr = "Compra";
        }
        else if (mov->getOperationType() == OperationType::SELL)
        {
            operationTypeStr = "Venda";
        }
        else
        {
            operationTypeStr = "?";
        }

        std::cout << mov->getDate() << "\t"
                  << operationTypeStr << "\t"
                  << mov->getQuantity() << "\t\t"
                  << std::fixed << std::setprecision(2) << quote->getQuote() << "\t\t"
                  << value << "\n";

        delete quote;
    }

    double balance = calculateWalletBalance(walletId);
    std::cout << "Saldo Atual: R$ " << std::fixed << std::setprecision(2) << balance << "\n";

    delete wallet;

    for (MovementDTO *mov_ptr : movements)
    {
        delete mov_ptr;
    }
}
