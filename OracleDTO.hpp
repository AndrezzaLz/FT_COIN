
#ifndef ORACLE_DTO_HPP
#define ORACLE_DTO_HPP

#include <string>
#include <stdexcept>

using namespace std;

class OracleDTO {
public:
    string data;
    double cotacao;

    OracleDTO(const string& data, double cotacao) {
        if (cotacao < 0.0) {
            throw invalid_argument("Cotação não pode ser negativa.");
        }
        if (!validarData(data)) {
            throw invalid_argument("Data inválida. Use o formato YYYY-MM-DD.");
        }
        this->data = data;
        this->cotacao = cotacao;
    }

private:
    bool validarData(const string& data) {
        return data.length() == 10 && data[4] == '-' && data[7] == '-';
    }
};

#endif
