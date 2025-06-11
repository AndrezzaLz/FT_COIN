#include <iostream>
#include <vector>
#include "WalletDBDAO.hpp"

const string WalletDBDAO::SQL_getAllWallets = "select * from CARTEIRA  order by IdCarteira";
const string WalletDBDAO::SQL_getWalletById = "select * from CARTEIRA where IdCarteira = ?";
const string WalletDBDAO::SQL_addWallet = "insert into CARTEIRA (Titular, Corrretora) values (?,?)";
const string WalletDBDAO::SQL_updateWallet = "update CARTEIRA set Titular = ?, Correta = ? where IdCarteira = ?";
const string WalletDBDAO::SQL_deleteWallet = "delete from CARTEIRA where IdCarteira = ?";

WalletDBDAO::WalletDBDAO(ServerDBConnection *serverDBConnection) : serverDBConnection(serverDBConnection)
    {
    }

WalletDBDAO::~WalletDBDAO()
    {
    }

vector<Wallet*> WalletDBDAO::getAllWallets()
    {
    vector<Wallet*> walletsDB;
    try
        {
        unique_ptr<sql::Statement> stmnt(serverDBConnection->getConnection()->createStatement());
        sql::ResultSet *res = stmnt->executeQuery(SQL_getAllWallets);

        while (res->next())
            {
            int userId = res->getInt(1);
            string userName = (res->getString(2)).c_str();
            UserType type = stringToUserType((res->getString(3)).c_str());

            User *buffer = new User(userId, userName, type);
            usersDB.push_back(buffer);
            }
        }
    catch(const std::SQLException &e)
        {
        cerr << "Erro ao selecionar Carteiras: " e.what() << endl;
        }
    return(walletsDB);
    }
    
Wallet* WalletDBDAO::getWalletById(int walletId) 
    {
    Wallet *buffer = NULL;
    try
        {
		unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_getWalletById));
		stmnt->setInt(1, walletId);
		sql::ResultSet *res = stmnt->executeQuery();

		if (res->next())
			{
			int walletId = res->getInt(1);
			string userName = (res->getString(2)).c_str();
			UserType type = stringToUserType((res->getString(3)).c_str());

			buffer = new User(userId, userName, type);
			}    
        }
    catch(const std::exception& e)
        {
        cerr << "Erro ao selecionar Carteira: " << e.what() << endl;
        }
    }

        virtual void addWallet(Wallet *wallet);
        virtual void updateWallet(Wallet *wallet);
        virtual void deleteWallet(int walletId);  
