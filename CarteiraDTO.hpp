
#ifndef CARTEIRA_DTO_HPP
#define CARTEIRA_DTO_HPP

#include <string>
#include <stdexcept>

using namespace std;

class CarteiraDTO {
public:
    int id;
    string nomeTitular;
    string corretora;

    CarteiraDTO(int id, const string& nomeTitular, const string& corretora) {
        if (nomeTitular.empty()) {
            throw invalid_argument("Nome do titular não pode estar vazio.");
        }
        if (corretora.empty()) {
            throw invalid_argument("Corretora não pode estar vazia.");
        }
        this->id = id;
        this->nomeTitular = nomeTitular;
        this->corretora = corretora;
    }
};

#endif
