#include "MovementMemDAO.hpp"

int MovementMemDAO::lastMovementId = 0;

MovementMemDAO::MovementMemDAO(MemoryDBConnection *memoryDBConnection) : memoryDBConnection(memoryDBConnection)
    {
    }

MovementMemDAO::~MovementMemDAO()
    {
    }

vector<MovementDTO*> MovememtMemDAO::getAllMovements() // const ?
    {
    return (memoryDBConnection->getMovementList());
    }

MovementDTO* MovementMemDAO::getMovementById(int movementId) // read
    {
    return (memoryDBConnection->getMovementById(movementId));

virtual void addMovement(MovementDTO *movement); // ou const &?
virtual void updateMovement(MovementDTO *movement); // ID + const &
virtual void deleteMovement(MovementDTO *movement); // remove ID