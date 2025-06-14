#include <iostream>
#include "OracleDBDAO.hpp"
#include "Date.hpp"

const string OracleDBDAO::SQL_saveQuote = "insert into ORACULO (Data, Cotacao) values (?, ?) on duplicate key update Cotacao = Values(Cotacao)";
const string OracleDBDAO::SQL_getQuote = "select Data, Cotacao from ORACULO where data = ?";

OracleDBDAO::OracleDBDAO(ServerDBConnection *serverDBConnection) :
        serverDBConnection(serverDBConnection)
    {
    }

OracleDBDAO::~OracleDBDAO()
    {
    }

void OracleDBDAO::saveQuote(OracleDTO *quote)
    {
    try
        {
        unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_saveQuote));

        stmnt->setString(1, quote->getDate().getIsoFormat());
        stmnt->setDouble(2, quote->getQuote());

        stmnt->executeQuery();
        }
    catch(sql::SQLException &e)
        {
        cerr << "Erro ao salvar cotacao: " << e.what() << endl;
        }
    }

OracleDTO* OracleDBDAO::getQuoteByDate(Date date)
    {
    OracleDTO *buffer = NULL;
    try
        {
        unique_ptr<sql::PreparedStatement> stmnt(serverDBConnection->getConnection()->prepareStatement(SQL_getQuote));
        stmnt->setString(1, date.getIsoFormat());
        sql::ResultSet *res = stmnt->executeQuery();

        if (res->next())
            {
            string resultDateStr(res->getString(1));
            Date dateObjectFromDB(resultDateStr);
            double quote = res->getDouble(2);

            buffer = new OracleDTO(dateObjectFromDB, quote);
            }
        }
    catch(sql::SQLException &e)
        {
        cerr << "Erro ao buscar cotacao: " << e.what() << endl;
        }
    return (buffer);
    }
