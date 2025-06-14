#ifndef ORACLEDBDAO_HPP
#define ORACLEDBDAO_HPP

#include <string>
#include "AbstractOracleDAO.hpp"
#include "ServerDBConnection.hpp"

class OracleDBDAO final : public AbstractOracleDAO
    {
    private:
        static const string SQL_saveQuote;
        static const string SQL_getQuote;

        ServerDBConnection *serverDBConnection;

    public:
        OracleDBDAO(ServerDBConnection *serverDBConnection);
        virtual ~OracleDBDAO();

        virtual void saveQuote(OracleDTO *quote);
        virtual OracleDTO* getQuoteByDate(Date date);
};

#endif
