#ifndef WALLETDBDAO_HPP
#define WALLETDBDAO_HPP

#include <string>
#include "AbstractWalletDAO.hpp"
#include "ServerDBConnection.hpp"

class WalletDBDAO final : public AbstractWalletDAO
{
private:
    static const string SQL_getAllWallets;
    static const string SQL_getWalletById;
    static const string SQL_addWallet;
    static const string SQL_updateWallet;
    static const string SQL_deleteWallet;

    ServerDBConnection *serverDBConnection;

public:
    WalletDBDAO(ServerDBConnection *serverDBConnection);
    virtual ~WalletDBDAO();

    virtual vector<WalletDTO *> getAllWallets();
    virtual WalletDTO *getWalletById(int walletId);
    virtual void addWallet(WalletDTO *wallet);
    virtual void updateWallet(WalletDTO *wallet);
    virtual void deleteWallet(int walletId);
    virtual void clearAll();
};

#endif