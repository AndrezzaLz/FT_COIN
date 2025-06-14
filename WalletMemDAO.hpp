#ifndef WALLETMEMDAO_HPP
#define WALLETMEMDAO_HPP

#include "AbstractWalletDAO.hpp"
#include "MemoryDBConnection.hpp"

class WalletMemDAO final : public AbstractWalletDAO
    {
    private:
        MemoryDBConnection *memoryDBConnection;
        static int lastWalletId;

    public:
        WalletMemDAO(MemoryDBConnection *memoryDBConnection);
        virtual ~WalletMemDAO();

        virtual vector<WalletDTO*> getAllWallets();
        virtual WalletDTO* getWalletById(int walletId);
        virtual void addWallet(WalletDTO *wallet);
        virtual void updateWallet(WalletDTO *wallet);
        virtual void deleteWallet(int walletId);
    };

#endif
