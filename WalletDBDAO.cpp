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

vector<WalletDTO*> WalletDBDAO::getAllWallets()
    {
    vector<WalletDTO*> walletsDB;
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
    catch(const sql::SQLException &e)
        {
        cerr << "Erro ao selecionar Carteiras: " << e.what() << endl;
        }
    return(walletsDB);
    }
    
WalletDTO* WalletDBDAO::getWalletById(int walletId)
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
    catch(sql::SQLException &e)
        {
        cerr << "Erro ao selecionar Carteiras: " << e.what() << endl;
        }
    return(buffer);
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
    catch(sql::SQLException &e)
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
    catch(sql::SQLException &e)
        {
        cerr << "Erro ao atualizar Carteira: " << e.what() << endl;
        }
    }

void WalletDBDAO::deleteWallet(int walletId)
    {
    try
        {
        unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_deleteWallet));
        stmnt->setInt(1, walletId);
        stmnt->executeQuery();
        }
    catch(sql::SQLException &e)
        {
        cerr << "Erro ao deletar Carteira: " << e.what() << endl;
        }
    }

#endif
