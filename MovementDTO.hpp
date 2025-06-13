#ifndef MOVEMENTDTO_HPP
#define MOVEMENTDTO_HPP

#include <string>
using namespace std;

class MovementDTO {
private:
    int movementId;
    int walletId;
    string date;
    char operationType; // 'C' for Compra, 'V' for Venda
    double quantity;

public:
    MovementDTO(int movementId, int walletId, const string& date, char operationType, double quantity);
    virtual ~MovementDTO();

    int getMovementId() const;
    int getWalletId() const;
    string getDate() const;
    char getOperationType() const;
    double getQuantity() const;
    void setMovementId(int movementId);
};

#endif
