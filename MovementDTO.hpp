#ifndef MOVEMENTDTO_HPP
#define MOVEMENTDTO_HPP

#include <string>
#include "Date.hpp"
using namespace std;

class MovementDTO {
private:
    int movementId;
    int walletId;
    Date date;
    char operationType; // 'C' for Compra, 'V' for Venda
    double quantity;

public:
    MovementDTO(int movementId, int walletId, const Date& date, char operationType, double quantity);
    virtual ~MovementDTO();

    int getMovementId() const;
    int getWalletId() const;
    Date getDate() const;
    char getOperationType() const;
    double getQuantity() const;
    void setMovementId(int movementId);
};

#endif
