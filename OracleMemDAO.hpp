#ifndef ORACLEMEMDAO_HPP
#define ORACLEMEMDAO_HPP

#include "AbstractOracleDAO.hpp"
#include "MemoryDBConnection.hpp"

class OracleMemDAO final : public AbstractOracleDAO
    {
    private: 
        MemoryDBConnection *memoryDBConnection;

    public:
        OracleMemDAO(MemoryDBConnection *memoryDBConnection);
        virtual ~OracleMemDAO();

        virtual OracleDTO* getQuoteByDate(Date date);
        virtual void saveQuote(OracleDTO *quote);
    };

#endif
