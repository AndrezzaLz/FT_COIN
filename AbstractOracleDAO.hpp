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

    virtual OracleDTO *getQuoteByDate(Date date) = 0;
    virtual void saveQuote(OracleDTO *quote) = 0;
    virtual void clearAll() = 0;
};

#endif
