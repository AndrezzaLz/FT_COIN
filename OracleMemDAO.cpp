#include "OracleMemDAO.hpp"
#include "MemoryDBConnection.hpp"
#include "OracleDTO.hpp"
#include "RecordNotFoundException.hpp" 
#include <iostream>
#include <algorithm>
#include <string> 

OracleMemDAO::OracleMemDAO(MemoryDBConnection* memoryDBConnection) : memoryDBConnection(memoryDBConnection) {}

OracleMemDAO::~OracleMemDAO() {}

OracleDTO* OracleMemDAO::getQuoteByDate(Date date) {
    auto it = memoryDBConnection->getOracleMap().find(date.getIsoFormat());

    if (it != memoryDBConnection->getOracleMap().end()) {
        return it->second;
    }
    
    throw RecordNotFoundException("Quote for date " + date.getIsoFormat() + " not found.");
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
