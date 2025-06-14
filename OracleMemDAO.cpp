#include "OracleMemDAO.hpp"
#include "MemoryDBConnection.hpp"
#include "OracleDTO.hpp"
#include <iostream>
#include <algorithm>

OracleMemDAO::OracleMemDAO(MemoryDBConnection* memoryDBConnection) : memoryDBConnection(memoryDBConnection) {}

OracleMemDAO::~OracleMemDAO() {}

OracleDTO* OracleMemDAO::getQuoteByDate(Date date) {
    auto it = memoryDBConnection->getOracleMap().find(date.getIsoFormat());
    if (it != memoryDBConnection->getOracleMap().end()) {
        return it->second;
    }
    return nullptr; // Retorna nullptr se a data não for encontrada
}

void OracleMemDAO::saveQuote(OracleDTO* quote) {
    string dateStr = quote->getDate().getIsoFormat();
    auto it = memoryDBConnection->getOracleMap().find(dateStr);
    if (it != memoryDBConnection->getOracleMap().end()) {
        delete it->second;
        it->second = quote;
    } else {
        memoryDBConnection->getOracleMap()[dateStr] = quote;
    }
}