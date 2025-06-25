#ifndef WALLETDTO_HPP
#define WALLETDTO_HPP

#include <string>
#include <stdexcept>

using namespace std;

class WalletDTO
{
private:
    int walletId;
    string holderName;
    string exchangeName;

public:
    WalletDTO(int walletId, const string &holderName, const string &exchangeName);
    virtual ~WalletDTO();

    int getWalletId() const;
    string getHolderName() const;
    string getExchangeName() const;
    void setWalletId(int walletId);
    void setHolderName(const string &holderName);
    void setExchangeName(const string &exchangeName);
};

#endif
