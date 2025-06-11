#ifndef ABSTRACTWALLETDAO_HPP
#define ABSTRACTWALLETDAO_HPP

#include <vector>
#include "CarteiraDTO.hpp"

class AbstractWalletDAO 
    {
    public:
        AbstractWalletDAO();
        virtual ~AbstractWalletDAO();

        virtual vector<Wallet*> getAllWallets() = 0;
        virtual Wallet* getWalletById(int walletId) = 0;
        virtual void addWallet(Wallet *wallet) = 0;
        virtual void updateWallet(Wallet *wallet) = 0;
        virtual void deleteWallet(int walletId) = 0;
    };

#endif 