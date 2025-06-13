#include <vector>
#include "WalletMemDAO.hpp"

int WalletMemDAO::lastWalletId = 1;

WalletMemDAO::WalletMemDAO(MemoryDBConnection *memoryConnection) : memoryDBConnection(memoryDBConnection)
    {
    }

WalletMemDAO::~WalletMemDAO()
    {
    }

vector<WalletDTO*> WalletMemDAO::getAllWallets()
    {
    return(memoryDBConnection->getWalletList());
    }

WalletDTO* WalletMemDAO::getWalletById(int walletId)
    {
    vector<WalletDTO*> &wallets = memoryDBConnection->getWalletList();
    vector<WalletDTO*>::iterator walletsIterator = wallets.begin();
    WalletDTO *buffer = NULL;
    bool found = false;

    while ((!found) && (walletsIterator != wallets.end()))
        {
        if ((*walletsIterator)->getWalletId() == walletId)
            {
                found = true;
                buffer = *walletsIterator;
            }
        walletsIterator++;
    }

    return (buffer);
    }

void WalletMemDAO::addWallet(WalletDTO *wallet)
    {
    wallet->setWalletId(++lastWalletId);
    (memoryDBConnection->getWalletList()).push_back(wallet);
    }

void WalletMemDAO::updateWallet(WalletDTO *wallet)
    {
    deleteWallet(wallet->getWalletId());
    addWallet(wallet);
    }

void WalletMemDAO::deleteWallet(int walletId)
    {
    vector<WalletDTO*> &wallets = memoryDBConnection->getWalletList();
    vector<WalletDTO*>::iterator walletsIterator = wallets.begin();
    bool found = false;

    while ((!found) && (walletsIterator != wallets.end()))
        {
        if ((*walletsIterator)->getWalletId() == walletId)
            {
                found = true;
                delete *walletsIterator;
                wallets.erase(walletsIterator);
            }
        walletsIterator++;
        }
    }