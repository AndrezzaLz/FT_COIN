#include "WalletDTO.hpp"
#include <stdexcept>

WalletDTO::WalletDTO(int walletId, const string& holderName, const string& exchangeName) :
        walletId(walletId), holderName(holderName),exchangeName(exchangeName)
    {
        if (holderName.empty()) {
            throw invalid_argument("Nome do titular não pode estar vazio.");
        }
        if (exchangeName.empty()) {
            throw invalid_argument("Corretora não pode estar vazia.");
        }
    }

WalletDTO::~WalletDTO()
    {
    }

int WalletDTO::getWalletId() const
    {
    return walletId;
    }

string WalletDTO::getHolderName() const
    {
    return holderName;
    }

string WalletDTO::getExchangeName() const
    {
    return exchangeName;
    }

void WalletDTO::setWalletId(int walletId)
    {
    if (this->walletId == 0)
        {
        this->walletId = walletId;
        }
    else
        {
        throw runtime_error("Operacao invalida: Alteracao do valor de IdCarteira que nao e nulo.");
        }
    }