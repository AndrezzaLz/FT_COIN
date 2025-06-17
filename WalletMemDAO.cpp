#include <vector>
#include <string>
#include "WalletMemDAO.hpp"
#include "RecordNotFoundException.hpp" 

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
    for (WalletDTO* wallet : wallets) {
        if (wallet->getWalletId() == walletId) {
            return wallet;
        }
    }
    throw RecordNotFoundException("Wallet with ID " + std::to_string(walletId) + " not found.");
    }

void WalletMemDAO::addWallet(WalletDTO *wallet)
    {
    wallet->setWalletId(++lastWalletId);
    (memoryDBConnection->getWalletList()).push_back(wallet);
    }

void WalletMemDAO::updateWallet(WalletDTO *wallet)
    {
    vector<WalletDTO*> &wallets = memoryDBConnection->getWalletList();
    for (auto it = wallets.begin(); it != wallets.end(); ++it) {
        if ((*it)->getWalletId() == wallet->getWalletId()) {
            delete *it; 
            *it = wallet; 
            return; 
        }
    }
    throw RecordNotFoundException("Wallet with ID " + std::to_string(wallet->getWalletId()) + " not found for update.");
    }

void WalletMemDAO::deleteWallet(int walletId)
    {
    vector<WalletDTO*> &wallets = memoryDBConnection->getWalletList();
    auto walletsIterator = wallets.begin();
    
    while (walletsIterator != wallets.end())
        {
        if ((*walletsIterator)->getWalletId() == walletId)
            {
                delete *walletsIterator; 
                wallets.erase(walletsIterator);
                return; 
            }
        else
            {
            ++walletsIterator;
            }
        }
    
    throw RecordNotFoundException("Wallet with ID " + std::to_string(walletId) + " not found for deletion.");
    }
