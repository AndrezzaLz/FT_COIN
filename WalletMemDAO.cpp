#include <vector>
#include "WalletMemDAO.hpp"

int WalletMemDAO::lastWalletId = 1;

WalletMemDAO::WalletMemDAO(MemoryDBConnection *memoryDBConnection)
{
    //Usei 'this->' para desambiguar e atribuir o parâmetro à variável 
    this->memoryDBConnection = memoryDBConnection;
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
    
    for (WalletDTO* walletPtr : wallets) {
        if (walletPtr->getWalletId() == walletId) {
            return new WalletDTO(*walletPtr);
        }
    }

    return nullptr; // Se não encontrar, retorna nulo.
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