#ifndef MOVEMENTMEMDAO_HPP
#define MOVEMENTMEMDAO_HPP

#include "AbstractMovementDAO.hpp"
#include "MemoryDBConnection.hpp"

class MovementMemDAO final : public AbstractMovementDAO
    {
    private:
        MemoryDBConnection *memoryDBConnection;
        static int lastMovementId;

    public:
        MovementMemDAO(MemoryDBConnection *memoryDBConnection);
        virtual ~MovementMemDAO();

        virtual vector<MovementDTO*> getAllMovements(); // const ?
        virtual MovementDTO* getMovementById(int movementId); // read
        virtual void addMovement(MovementDTO *movement); // ou const &?
        virtual void updateMovement(MovementDTO *movement); // ID + const &
        virtual void deleteMovement(MovementDTO *movement); // remove ID
    };

#endif



