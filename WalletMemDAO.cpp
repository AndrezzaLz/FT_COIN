#include <vector>
#include "WalletMemDAO.hpp"
#include "Utils.h"

int WalletMemDAO::lastWalletId = 1;

WalletMemDAO::WalletMemDAO(MemoryDBConnection *memoryDBConnection)
{
    this->memoryDBConnection = memoryDBConnection;
}

WalletMemDAO::~WalletMemDAO()
{
}

vector<WalletDTO *> WalletMemDAO::getAllWallets()
{
    vector<WalletDTO *> wallets_copy;
    for (WalletDTO *walletPtr : memoryDBConnection->getWalletList())
    {
        wallets_copy.push_back(new WalletDTO(*walletPtr));
    }
    return wallets_copy;
}

WalletDTO *WalletMemDAO::getWalletById(int walletId)
{
    vector<WalletDTO *> &wallets = memoryDBConnection->getWalletList();

    for (WalletDTO *walletPtr : wallets)
    {
        if (walletPtr->getWalletId() == walletId)
        {
            return new WalletDTO(*walletPtr);
        }
    }

    return nullptr;
}
void WalletMemDAO::addWallet(WalletDTO *wallet)
{
    wallet->setWalletId(++lastWalletId);
    (memoryDBConnection->getWalletList()).push_back(wallet);
}

void WalletMemDAO::updateWallet(WalletDTO *wallet)
{
    vector<WalletDTO *> &wallets = memoryDBConnection->getWalletList();
    bool found = false;

    for (WalletDTO *existingWalletPtr : wallets)
    {
        if (existingWalletPtr->getWalletId() == wallet->getWalletId())
        {
            existingWalletPtr->setHolderName(wallet->getHolderName());
            existingWalletPtr->setExchangeName(wallet->getExchangeName());
            found = true;
            delete wallet;
            break;
        }
    }
    if (!found)
    {
        Utils::printMessage("Aviso: Carteira nao encontrada para atualizacao em memoria.");
        delete wallet;
    }
}

void WalletMemDAO::deleteWallet(int walletId)
{
    vector<WalletDTO *> &wallets = memoryDBConnection->getWalletList();
    vector<WalletDTO *>::iterator walletsIterator = wallets.begin();
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

void WalletMemDAO::clearAll()
{
    for (WalletDTO *buffer : memoryDBConnection->getWalletList())
    {
        delete buffer;
    }
    memoryDBConnection->getWalletList().clear();
    lastWalletId = 0;
}
