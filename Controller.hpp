#ifndef Controller_hpp
#define Controller_hpp

#include <string>
#include <vector>
#include <iostream> // Para cin/cout

// Inclui seus DAOs abstratos
#include "AbstractWalletDAO.hpp"
#include "AbstractMovementDAO.hpp"
#include "AbstractOracleDAO.hpp"

// Inclui conexões e outras dependências
#include "DataBaseSelector.hpp"
#include "MemoryDBConnection.hpp"
#include "ServerDBConnection.hpp"
#include "Menu.h" // A classe Menu
#include "Utils.h"
#include "SysInfo.h"
#include "TextFromFile.h"

// Inclui DTOs necessários para as operações do Controller
#include "WalletDTO.hpp"
#include "MovementDTO.hpp"
#include "OracleDTO.hpp"
#include "Date.hpp"

class Controller final {
private:
    MemoryDBConnection *memoryDBConnection;
    ServerDBConnection *serverDBConnection;
    AbstractWalletDAO *walletDAO;
    AbstractMovementDAO *movementDAO;
    AbstractOracleDAO *oracleDAO;
    BusinessLogic *businessLogic;

    // Métodos para as ações dos menus principais
    void actionWallet();
    void actionMovement();
    void actionReports();
    void actionHelp();
    void actionAbout();
    
    // Método utilitário para lançar menus
    void launchActions(std::string title, std::vector<std::string> menuItens, std::vector<void (Controller::*)()> functions);

    // Métodos específicos para funcionalidades da Carteira
    void newWallet();
    void retrieveWallet();
    void editWallet();
    void deleteWallet();
    void listWalletsById();
    void listWalletsByHolderName();
    
    // Métodos específicos para funcionalidades de Movimentação
    void registerPurchase();
    void registerSale();

    // Métodos específicos para funcionalidades de Relatórios
    void showWalletBalance();
    void showMovementHistory();
    void showGainsLosses();

    // Métodos específicos para Ajuda
    void showHelpText();
    void showCredits(); // Chama actionAbout

    // Método para popular dados de demonstração (para DAO em memória)
    void populateDemoData();

public:
    Controller(DataBaseSelector dataBaseSelector);
    virtual ~Controller();
    void start();
};

#endif // CONTROLLER_HPP