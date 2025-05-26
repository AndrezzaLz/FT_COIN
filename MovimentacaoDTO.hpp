
#ifndef MOVIMENTACAO_DTO_HPP
#define MOVIMENTACAO_DTO_HPP

#include <string>
#include <stdexcept>

using namespace std;

class MovimentacaoDTO {
public:
    int id;
    int idCarteira;
    string data;
    char tipoOperacao;
    double quantidade;

    MovimentacaoDTO(int id, int idCarteira, const string& data, char tipoOperacao, double quantidade) {
        if (quantidade < 0.0) {
            throw invalid_argument("Quantidade deve ser maior ou igual a zero.");
        }
        if (tipoOperacao != 'C' && tipoOperacao != 'V') {
            throw invalid_argument("Tipo de operação inválido. Use 'C' ou 'V'.");
        }
        if (!validarData(data)) {
            throw invalid_argument("Data inválida. Use o formato YYYY-MM-DD.");
        }
        this->id = id;
        this->idCarteira = idCarteira;
        this->data = data;
        this->tipoOperacao = tipoOperacao;
        this->quantidade = quantidade;
    }

private:
    bool validarData(const string& data) {
        return data.length() == 10 && data[4] == '-' && data[7] == '-';
    }
};

#endif
