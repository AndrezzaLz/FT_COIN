#include <stdexcept>
#include "OracleDTO.hpp"

OracleDTO::OracleDTO(const string& date, double quote)
    {
        if (quote < 0.0) {
            throw invalid_argument("Cotacao nao pode ser negativa.");
        }
        if (!isDateValid(date)) {
            throw invalid_argument("Data invalida. Use o formato YYYY-MM-DD.");
        }
        this->date = date;
        this->quote = quote;
    }

OracleDTO::~OracleDTO()
    {
    }

string OracleDTO::getDate() const
    {
    return this->date;
    }

double OracleDTO::getQuote() const
    {
    return this->quote;
    }

bool OracleDTO::isDateValid(const string& date) {
    return date.length() == 10 && date[4] == '-' && date[7] == '-';
}
