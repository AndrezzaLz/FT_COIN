
#ifndef MOVIMENTACAODTO_HPP
#define MOVIMENTACAODTO_HPP

#include <string>

class MovimentacaoDTO {
public:
    int id;
    int idCarteira;
    std::string data;
    char tipoOperacao;
    double quantidade;

    // ✅ Construtor padrão (necessário para containers e compatibilidade com C++11)
    MovimentacaoDTO()
        : id(0), idCarteira(0), data(""), tipoOperacao('C'), quantidade(0.0) {}

    // ✅ Construtor parametrizado
    MovimentacaoDTO(int id_, int idCarteira_, const std::string& data_, char tipo_, double quantidade_)
        : id(id_), idCarteira(idCarteira_), data(data_), tipoOperacao(tipo_), quantidade(quantidade_) {}
};

#endif // MOVIMENTACAODTO_HPP
