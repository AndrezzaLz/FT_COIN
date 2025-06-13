#ifndef ABSTRACTORACLEDAO_HPP
#define ABSTRACTORACLEDAO_HPP

#include <vector>
#include <string>
#include "OracleDTO.hpp"

class AbstractOracleDAO 
    {
    public:
        AbstractOracleDAO();
        virtual ~AbstractOracleDAO();

        virtual OracleDTO* getQuoteByDate(string date) = 0;
        virtual void saveQuote(const OracleDTO& quote) = 0;
    };

#endif 