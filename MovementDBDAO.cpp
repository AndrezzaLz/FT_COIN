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
        stmnt->setString(3, string(1, static_cast<char>(movement->getOperationType())));
        stmnt->setDouble(4, movement->getQuantity());
        stmnt->executeQuery();
    }
    catch (sql::SQLException &e)
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
            string dateStr(res->getString(3));
            Date date(dateStr);
            OperationType operationType = static_cast<OperationType>(res->getString(4).at(0));
            double quantity = res->getDouble(5);

            MovementDTO *buffer = new MovementDTO(movementId, walletId, date, operationType, quantity);
            historyDB.push_back(buffer);
        }
    }
    catch(sql::SQLException &e)
    {
        cerr << "Erro ao buscar historico: " << e.what() << endl;
    }
    return historyDB;
}

void MovementDBDAO::clearAll() {
    try 
    {
        unique_ptr<sql::Statement> stmnt(serverDBConnection->getConnection()->createStatement());
        stmnt->executeQuery("delete from MOVIMENTACAO"); 
    } 
    catch(sql::SQLException &e) 
    {
        cerr << "Erro ao limpar tabela MOVIMENTACAO: " << e.what() << endl;
    } 
}

#endif
