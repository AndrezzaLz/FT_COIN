#ifdef USE_MARIADB

#include <iostream>
#include "MovementDBDAO.hpp"

const string MovementDBDAO::SQL_registerTransaction = "insert into MOVIMENTACAO (IdCarteira, Data, TipoOperacao, Quantidade) values (?, ?, ?, ?)";
const string MovementDBDAO::SQL_getHistory = "select IdMovimento, IdCarteira, Data, TipoOperacao, Quantidade from MOVIMENTACAO where IdCarteira = ? order by Data";

MovementDBDAO::MovementDBDAO(ServerDBConnection *serverDBConnection) : serverDBConnection(serverDBConnection)
{
}

MovementDBDAO::~MovementDBDAO()
{
}

void MovementDBDAO::registerTransaction(MovementDTO *movement)
{
    try
    {
        unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_registerTransaction));
        stmnt->setInt(1, movement->getWalletId());
        stmnt->setString(2, movement->getDate().getIsoFormat());
        stmnt->setString(3, string(1, movement->getOperationType()));
        stmnt->setDouble(4, movement->getQuantity());
        stmnt->executeQuery();
    }
    catch (sql::SQLException &e)
    {
        cerr << "Erro ao registrar movimentacao: " << e.what() << endl;
    }
}

vector<MovementDTO *> MovementMemDAO::getHistoryByWalletId(int walletId)
{
    vector<MovementDTO *> history;
    for (MovementDTO *movementPtr : memoryDBConnection->getMovementList())
    {
        if (movementPtr->getWalletId() == walletId)
        {
            history.push_back(new MovementDTO(*movementPtr));
        }
    }

    sort(history.begin(), history.end(), [](MovementDTO *a, MovementDTO *b)
         { return a->getDate() < b->getDate(); });

    return history;

#endif
