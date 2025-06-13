#include "OracleMemDAO.hpp"
#include "MemoryDBConnection.hpp"
#include "OracleDTO.hpp"
#include <iostream>
#include <algorithm>

OracleMemDAO::OracleMemDAO(MemoryDBConnection* memoryDBConnection) : memoryDBConnection(memoryDBConnection) {}

OracleMemDAO::~OracleMemDAO() {}

OracleDTO* OracleMemDAO::getQuoteByDate(Date date) {
    auto it = memoryDBConnection->getOracleDB().find(date.getIsoFormat());
    if (it != memoryDBConnection->getOracleDB().end()) {
        return it->second;
    }
    return nullptr; // Retorna nullptr se a data não for encontrada
}

void OracleMemDAO::saveQuote(OracleDTO* quote) {
    std::string dateStr = quote->getDate().getIsoFormat();
    auto it = memoryDBConnection->getOracleDB().find(dateStr);
    if (it != memoryDBConnection->getOracleDB().end()) {
        // A data já existe, atualiza a cotação
        it->second = quote;
    } else {
        // A data não existe, adiciona a nova cotação
        memoryDBConnection->getOracleDB()[dateStr] = quote;
    }
}