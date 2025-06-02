#ifndef DAO_HPP
#define DAO_HPP

#include "MovimentacaoDTO.hpp"
#include <vector>

class DAO {
public:
    virtual void create(const MovimentacaoDTO& m) = 0;
    virtual MovimentacaoDTO read(int movimentoID) const = 0;
    virtual void update(int movimentoID, const MovimentacaoDTO& m) = 0;
    virtual void remove(int movimentoID) = 0;
    virtual std::vector<MovimentacaoDTO> listAll() const = 0;
    virtual ~DAO() = default;
};

#endif
