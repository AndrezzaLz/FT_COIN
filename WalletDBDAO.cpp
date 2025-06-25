#ifdef USE_MARIADB

#include <iostream>
#include <vector>
#include "WalletDBDAO.hpp"

const string WalletDBDAO::SQL_getAllWallets = "select * from CARTEIRA order by IdCarteira";
const string WalletDBDAO::SQL_getWalletById = "select * from CARTEIRA where IdCarteira = ?";
const string WalletDBDAO::SQL_addWallet = "insert into CARTEIRA (Titular, Corretora) values (?,?)";
const string WalletDBDAO::SQL_updateWallet = "update CARTEIRA set Titular = ?, Corretora = ? where IdCarteira = ?";
const string WalletDBDAO::SQL_deleteWallet = "delete from CARTEIRA where IdCarteira = ?";

WalletDBDAO::WalletDBDAO(ServerDBConnection *serverDBConnection) : serverDBConnection(serverDBConnection)
{
}

WalletDBDAO::~WalletDBDAO()
{
}

vector<WalletDTO *> WalletDBDAO::getAllWallets()
{
    vector<WalletDTO *> walletsDB;
    try
    {
        unique_ptr<sql::Statement> stmnt(serverDBConnection->getConnection()->createStatement());
        sql::ResultSet *res = stmnt->executeQuery(SQL_getAllWallets);

        while (res->next())
        {
            int walletId = res->getInt(1);
            string holderName = (res->getString(2)).c_str();
            string exchangeName = (res->getString(3)).c_str();

            WalletDTO *buffer = new WalletDTO(walletId, holderName, exchangeName);
            walletsDB.push_back(buffer);
        }
    }
    catch (const sql::SQLException &e)
    {
        cerr << "Erro ao selecionar Carteiras: " << e.what() << endl;
    }
    return (walletsDB);
}

WalletDTO *WalletDBDAO::getWalletById(int walletId)
{
    WalletDTO *buffer = NULL;
    try
    {
        unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_getWalletById));
        stmnt->setInt(1, walletId);
        sql::ResultSet *res = stmnt->executeQuery();

        if (res->next())
        {
            int walletId = res->getInt(1);
            string holderName(res->getString(2));
            string exchangeName(res->getString(3));

            buffer = new WalletDTO(walletId, holderName, exchangeName);
        }
    }
    catch (sql::SQLException &e)
    {
        cerr << "Erro ao selecionar Carteiras: " << e.what() << endl;
    }
    return (buffer);
}

void WalletDBDAO::addWallet(WalletDTO *wallet)
{
    try
    {
        unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_addWallet));
        stmnt->setString(1, wallet->getHolderName());
        stmnt->setString(2, wallet->getExchangeName());
        stmnt->executeQuery();
    }
    catch (sql::SQLException &e)
    {
        cerr << "Erro ao inserir nova Carteira: " << e.what() << endl;
    }
}

void WalletDBDAO::updateWallet(WalletDTO *wallet)
{
    try
    {
        unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_updateWallet));
        stmnt->setString(1, wallet->getHolderName());
        stmnt->setString(2, wallet->getExchangeName());
        stmnt->setInt(3, wallet->getWalletId());
        stmnt->executeQuery();
    }
    catch (sql::SQLException &e)
    {
        cerr << "Erro ao atualizar Carteira: " << e.what() << endl;
    }
}

void WalletDBDAO::deleteWallet(int walletId)
{
    try
    {
        serverDBConnection->getConnection()->setAutoCommit(false);

        // Exclusão das movimentações associadas a Carteira devido à relação de Chave Estragengeira de Moviventação
        unique_ptr<sql::PreparedStatement> stmntDeleteMovement(serverDBConnection->getConnection()->prepareStatement("delete from MOVIMENTACAO where IdCarteira = ?"));
        stmntDeleteMovement->setInt(1, walletId);
        stmntDeleteMovement->executeQuery();

        unique_ptr<sql::PreparedStatement> stmntDeleteWallet(serverDBConnection->getConnection()->prepareStatement(SQL_deleteWallet));
        stmntDeleteWallet->setInt(1, walletId);
        stmntDeleteWallet->executeQuery();

        serverDBConnection->getConnection()->commit();
        serverDBConnection->getConnection()->setAutoCommit(true);
    }
    catch (sql::SQLException &e)
    {
        try
        {
            serverDBConnection->getConnection()->rollback();
            serverDBConnection->getConnection()->setAutoCommit(true);
        }
        catch (const std::exception &rb_e)
        {
            cerr << "Erro no rollback: " << rb_e.what() << endl;
        }
        cerr << "Erro ao deletar Carteira e/ou suas Movimentacoes: " << e.what() << endl;
    }
}

void WalletDBDAO::clearAll()
{
    try
    {
        unique_ptr<sql::Statement> stmnt(serverDBConnection->getConnection()->createStatement());
        stmnt->executeQuery("delete from CARTEIRA");
    }
    catch (sql::SQLException &e)
    {
        cerr << "Erro ao limpar tabela CARTEIRA: " << e.what() << endl;
    }
}

#endif
