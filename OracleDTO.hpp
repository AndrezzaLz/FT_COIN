#ifndef ORACLEDTO_HPP
#define ORACLEDTO_HPP

#include <string>
using namespace std;

class OracleDTO {
private:
    string date;
    double quote;

public:
    OracleDTO(const string& date, double quote);
    virtual ~OracleDTO();

    string getDate() const;
    double getQuote() const;
    bool isDateValid(const string& date);
};

#endif
