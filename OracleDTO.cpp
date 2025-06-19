#include <stdexcept>
#include "OracleDTO.hpp"

OracleDTO::OracleDTO(Date date, double quote) : date(date), quote(quote)
    {
        if (quote < 0.0) {
            throw invalid_argument("Cotacao nao pode ser negativa.");
        }
    }

OracleDTO::~OracleDTO()
    {
    }

Date OracleDTO::getDate() const
    {
    return this->date;
    }

double OracleDTO::getQuote() const
    {
    return this->quote;
    }
