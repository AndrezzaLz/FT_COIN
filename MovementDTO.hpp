#ifndef MOVEMENTDTO_HPP
#define MOVEMENTDTO_HPP

#include <string>
#include "Date.hpp"
using namespace std;

enum class OperationType
{
    BUY = 'C', // 'C' for Buy (Compra - PTBR)
    SELL = 'V' // 'V' for SELL (Venda - PTBR)
};

class MovementDTO
{
private:
    int movementId;
    int walletId;
    Date date;
    OperationType operationType; // 'C' for Compra, 'V' for Venda
    double quantity;

public:
    MovementDTO(int movementId, int walletId, const Date &date, OperationType operationType, double quantity);
    virtual ~MovementDTO();
    MovementDTO(const MovementDTO &other) = default;

    int getMovementId() const;
    int getWalletId() const;
    Date getDate() const;
    OperationType getOperationType() const;
    double getQuantity() const;
    void setMovementId(int movementId);
};

#endif
