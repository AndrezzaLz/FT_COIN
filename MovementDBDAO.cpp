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
        stmnt->setString(2, movement->getDate());
        stmnt->setString(3, string(1, movement->getOperationType()));
        stmnt->setDouble(4, movement->getQuantity());
        stmnt->executeQuery();
        }
    catch(sql::SQLException &e)
        {
        cerr << "Erro ao registrar movimentacao: " << e.what() << endl;
        }
    }

vector<MovementDTO*> MovementDBDAO::getHistoryByWalletId(int walletId)
    {
    vector<MovementDTO*> historyDB;
    try
        {
        unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_getHistory));
        stmnt->setInt(1, walletId);
        sql::ResultSet *res = stmnt->executeQuery();

        while (res->next())
            {
            int movementId = res->getInt(1);
            int WalletId = res->getInt(2);
            string date = res->getString(3);
            char operationType = res->getString(4);
            double quantity = res->getDouble(5);

            MovementDTO *buffer = new MovementDTO(movementId, walletId, date, operationtType, quantity);
            historyDB.push_back(buffer);
            }
        }
    catch(sql::SQLException &e)
        {
        cerr << "Erro ao buscar historico: " << e.what() << endl;
        }
        return historyDB;
    }



