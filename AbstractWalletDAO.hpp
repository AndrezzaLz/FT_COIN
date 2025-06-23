#ifndef ABSTRACTWALLETDAO_HPP
#define ABSTRACTWALLETDAO_HPP

#include <vector>
#include "WalletDTO.hpp"

class AbstractWalletDAO
{
public:
    AbstractWalletDAO();
    virtual ~AbstractWalletDAO();

    virtual vector<WalletDTO *> getAllWallets() = 0;
    virtual WalletDTO *getWalletById(int walletId) = 0;
    virtual void addWallet(WalletDTO *wallet) = 0;
    virtual void updateWallet(WalletDTO *wallet) = 0;
    virtual void deleteWallet(int walletId) = 0;
};

#endif