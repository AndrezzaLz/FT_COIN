#include "OracleMemDAO.hpp"
#include "MemoryDBConnection.hpp"
#include "OracleDTO.hpp"
#include "RegistroNaoEncontradoException.hpp" // ADICIONADO
#include <iostream>
#include <algorithm>
#include <string> // ADICIONADO

OracleMemDAO::OracleMemDAO(MemoryDBConnection* memoryDBConnection) : memoryDBConnection(memoryDBConnection) {}

OracleMemDAO::~OracleMemDAO() {}

// MÉTODO MODIFICADO
OracleDTO* OracleMemDAO::getQuoteByDate(Date date) {
    auto it = memoryDBConnection->getOracleMap().find(date.getIsoFormat());

    if (it != memoryDBConnection->getOracleMap().end()) {
        return it->second;
    }
    
    // Se não encontrou, lança a exceção
    throw RegistroNaoEncontradoException("Cotacao para a data " + date.getIsoFormat() + " nao encontrada.");
}

void OracleMemDAO::saveQuote(OracleDTO* quote) {
    std::string dateStr = quote->getDate().getIsoFormat();
    auto it = memoryDBConnection->getOracleMap().find(dateStr);
    
    if (it != memoryDBConnection->getOracleMap().end()) {
        delete it->second;
        it->second = quote;
    } else {
        memoryDBConnection->getOracleMap()[dateStr] = quote;
    }
}
