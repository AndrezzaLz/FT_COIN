#include "MovementDTO.hpp"
#include <stdexcept>

MovementDTO::MovementDTO(int movementId, int walletId, const Date& date, char operationType, double quantity) :
    movementId(movementId), walletId(walletId), date(date), operationType(operationType), quantity(quantity)
    {
        if (quantity < 0) {
            throw invalid_argument("A quantidade nao pode ser negativa.");
    }
    // Poderia haver mais validações aqui (data, tipo de operação, etc.)
}

MovementDTO::~MovementDTO()
    {
    }

int MovementDTO::getMovementId() const
    {
    return movementId;
    }

int MovementDTO::getWalletId() const
    {
    return walletId;
    }

Date MovementDTO::getDate() const
    {
    return date;
    }

char MovementDTO::getOperationType() const
    {
    return operationType;
    }

double MovementDTO::getQuantity() const
    {
    return quantity;
    }

void MovementDTO::setMovementId(int movementId)
    {
    if (this->movementId == 0)
        {
        this->movementId = movementId;
        }
    else
        {
        throw runtime_error("Operacao invalida: Alteracao do valor de IdMovimento que nao e nulo.");
        }
    }
