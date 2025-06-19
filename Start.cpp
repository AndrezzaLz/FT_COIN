#include <iostream>
#include <string>
#include <exception>
#include <memory>

// Inclua os cabeçalhos das suas classes principais
#include "Controller.hpp"
#include "DataBaseSelector.hpp"
#include "SysInfo.h"
#include "Utils.h"

// Funções de boas-vindas e encerramento
void welcome();
void bye();

int main(int argc, char *argv[])
{
    welcome();

    try
    {
        // Instancia o Controller. Para o vídeo, use DataBaseSelector::MEMORY.
        // Para usar o MariaDB, mude para DataBaseSelector::MARIADB.
    std::unique_ptr<Controller> appController(new Controller(DataBaseSelector::MEMORY));
    appController->start();        
        // Inicia o loop principal do programa
        appController->start();
    }
    catch (const std::exception &e)
    {
        Utils::printMessage("Problema inesperado. Programa abortado. " + std::string(e.what()));
        return 1;
    }

    bye();
    return 0;
}

// Implementação das funções
void welcome()
{
    Utils::printMessage(SysInfo::getFullVersion());
    Utils::printMessage("Iniciando FT_Coin...");
}

void bye()
{
    Utils::printMessage("FT_Coin encerrado. Ate mais!");
}