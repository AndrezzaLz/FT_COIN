#include <iostream>
#include <string>
#include <exception>
#include <memory>
#include <limits>
#include "Controller.hpp"
#include "DataBaseSelector.hpp"
#include "SysInfo.h"
#include "Utils.h"

void welcome();
void bye();

int main(int argc, char *argv[])
{
    welcome();

    DataBaseSelector selector;
    int choice = -1;

    std::cout << "Selecione o modo de persistencia:" << std::endl;
    std::cout << "1. Banco de Dados em Memoria" << std::endl;
    std::cout << "2. Banco de Dados MariaDB (requer servidor)" << std::endl;

    while (true)
    {
        std::cout << "Sua opcao: ";
        std::cin >> choice;

        if (std::cin.good() && (choice == 1 || choice == 2))
        {
            break;
        }
        else
        {
            std::cout << "Opcao invalida. Por favor, digite 1 ou 2." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 1)
    {
        selector = DataBaseSelector::MEMORY;
        Utils::printMessage("Modo de persistencia em MEMORIA selecionado.");
    }
    else
    {
        selector = DataBaseSelector::MARIADB;
    }

    try
    {
        std::unique_ptr<Controller> appController(new Controller(selector));

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

void welcome()
{
    Utils::printMessage(SysInfo::getFullVersion());
    Utils::printMessage("Iniciando FT_Coin...");
}

void bye()
{
    Utils::printMessage("FT_Coin encerrado. Ate mais!");
}