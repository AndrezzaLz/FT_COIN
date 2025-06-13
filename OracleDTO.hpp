#ifndef ORACLEDTO_HPP
#define ORACLEDTO_HPP

#include <string>
#include "Date.hpp"
using namespace std;

class OracleDTO {
private:
    Date date;
    double quote;

public:
    OracleDTO(Date date, double quote);
    virtual ~OracleDTO();

    Date getDate() const;
    double getQuote() const;
};

#endif
