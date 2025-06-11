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

        virtual Oracle* getQuoteByDate(string date) = 0;
        virtual void saveQuote(Oracle quote) = 0;
        virtual vector<Oracle*> getAllQuotes() = 0;
    };

#endif 