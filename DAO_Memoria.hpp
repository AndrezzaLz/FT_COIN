#ifndef DAO_MEMORIA_HPP
#define DAO_MEMORIA_HPP

#include "DAO.hpp"
#include <map>
#include <vector>
#include <stdexcept>  // para std::runtime_error

class DAO_Memoria : public DAO {
private:
    std::map<int, MovimentacaoDTO> movimentacoes;
    int nextMovimentoID = 1;

public:
    void create(const MovimentacaoDTO& m) override {
        MovimentacaoDTO copia = m;
        copia.id = nextMovimentoID++;
        movimentacoes[copia.id] = copia;
    }

    MovimentacaoDTO read(int movimentoID) const override {
        auto it = movimentacoes.find(movimentoID);
        if (it != movimentacoes.end()) {
            return it->second;
        }
        throw std::runtime_error("Movimentação não encontrada.");
    }

    void update(int movimentoID, const MovimentacaoDTO& m) override {
        if (movimentacoes.count(movimentoID)) {
            movimentacoes[movimentoID] = m;
        } else {
            throw std::runtime_error("Movimentação não encontrada.");
        }
    }

    void remove(int movimentoID) override {
        movimentacoes.erase(movimentoID);
    }

    std::vector<MovimentacaoDTO> listAll() const override {
        std::vector<MovimentacaoDTO> lista;
        for (std::map<int, MovimentacaoDTO>::const_iterator it = movimentacoes.begin(); it != movimentacoes.end(); ++it) {
            lista.push_back(it->second);
        }
        return lista;
    }
};

#endif
