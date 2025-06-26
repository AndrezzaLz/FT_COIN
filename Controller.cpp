#include "Controller.hpp"
#include "Utils.h"
#include <cctype>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <fstream>

#include "WalletMemDAO.hpp"
#include "MovementMemDAO.hpp"
#include "OracleMemDAO.hpp"
#include "BusinessLogic.hpp"

#ifdef USE_MARIADB
#include "ServerDBConnection.hpp"
#include "WalletDBDAO.hpp"
#include "MovementDBDAO.hpp"
#include "OracleDBDAO.hpp"
#endif

using namespace std;

Controller::Controller(DataBaseSelector dataBaseSelector)
{
    memoryDBConnection = nullptr;
    serverDBConnection = nullptr;
    walletDAO = nullptr;
    movementDAO = nullptr;
    oracleDAO = nullptr;

    switch (dataBaseSelector)
    {
    case DataBaseSelector::MEMORY:
    {
        memoryDBConnection = new MemoryDBConnection();

        walletDAO = new WalletMemDAO(memoryDBConnection);
        movementDAO = new MovementMemDAO(memoryDBConnection);
        oracleDAO = new OracleMemDAO(memoryDBConnection);
        businessLogic = new BusinessLogic(walletDAO, movementDAO, oracleDAO);

        populateDemoData();
    }
    break;

    case DataBaseSelector::MARIADB:
    {
#ifdef USE_MARIADB
        try
        {
            serverDBConnection = new ServerDBConnection();
            walletDAO = new WalletDBDAO(serverDBConnection);
            movementDAO = new MovementDBDAO(serverDBConnection);
            oracleDAO = new OracleDBDAO(serverDBConnection);
            businessLogic = new BusinessLogic(walletDAO, movementDAO, oracleDAO);

            populateDemoData();
        }
        catch (const exception &e)
        {
            cerr << "ERRO FATAL: Falha ao conectar ao MariaDB ou instanciar DAOs: " << e.what() << endl;
            exit(1);
        }
#else
        cerr << "ERRO: O programa nao foi compilado com suporte a MariaDB." << endl;
        exit(1);
#endif
    }
    break;
    default:
    {
        throw invalid_argument("Seletor de banco de dados invalido.");
    }
    }
}

Controller::~Controller()
{
    delete walletDAO;
    walletDAO = nullptr;
    delete movementDAO;
    movementDAO = nullptr;
    delete oracleDAO;
    oracleDAO = nullptr;
    delete businessLogic;
    businessLogic = nullptr;

    if (memoryDBConnection)
    {
        delete memoryDBConnection;
        memoryDBConnection = nullptr;
    }

#ifdef USE_MARIADB
    if (serverDBConnection)
    {
        delete serverDBConnection;
        serverDBConnection = nullptr;
    }
#endif
}

void Controller::start()
{
    vector<string> mainMenuItems;
    mainMenuItems.push_back("Carteira");
    mainMenuItems.push_back("Movimentacao");
    mainMenuItems.push_back("Relatorios");
    mainMenuItems.push_back("Ajuda");
    mainMenuItems.push_back(string(Utils::ANSI_RED) + "Sair do Programa" + Utils::ANSI_RESET);

    vector<void (Controller::*)()> functions = {
        &Controller::actionWallet,
        &Controller::actionMovement,
        &Controller::actionReports,
        &Controller::actionHelp};
    launchActions("Menu Principal FT_Coin", mainMenuItems, functions);
}

void Controller::actionWallet()
{
    vector<string> menuItens = {"Incluir Carteira", "Recuperar Carteira", "Editar Carteira", "Excluir Carteira", "Listar por ID", "Listar por Titular", "Voltar"};
    vector<void (Controller::*)()> functions = {
        &Controller::newWallet,
        &Controller::retrieveWallet,
        &Controller::editWallet,
        &Controller::deleteWallet,
        &Controller::listWalletsById,
        &Controller::listWalletsByHolderName};
    launchActions("Gerenciar Carteiras", menuItens, functions);
}

void Controller::actionMovement()
{
    vector<string> menuItens = {"Registrar Compra", "Registrar Venda", "Voltar"};
    vector<void (Controller::*)()> functions = {
        &Controller::registerPurchase,
        &Controller::registerSale};
    launchActions("Gerenciar Movimentacoes", menuItens, functions);
}

void Controller::actionReports()
{
    // Adicionamos a nova opção "Exportar"
    vector<string> menuItens = {"Exibir Saldo da Carteira", "Exibir Historico de Movimentacao", "Exibir Ganhos/Perdas", "Exportar Historico para CSV", "Voltar"};

    vector<void (Controller::*)()> functions = {
        &Controller::showWalletBalance,
        &Controller::showMovementHistory,
        &Controller::showGainsLosses,
        &Controller::exportHistoryToCSV 
    };
    launchActions("Relatorios", menuItens, functions);
}
void Controller::actionHelp()
{
    vector<string> menuItens = {"Mostrar Texto de Ajuda", "Mostrar Creditos", "Voltar"};
    vector<void (Controller::*)()> functions = {
        &Controller::showHelpText,
        &Controller::showCredits};
    launchActions("Ajuda e Informacoes", menuItens, functions);
}

void Controller::actionAbout()
{
    showCredits();
}

void Controller::launchActions(string title, vector<string> menuItens, vector<void (Controller::*)()> functions)
{
    try
    {
        Menu menu(menuItens, title, "Sua opcao: ");
        menu.setSymbol("*");
        menu.setZeroForLastOpt(true);

        while (int choice = menu.getChoice())
        {
            if (choice > 0 && choice <= (int)functions.size())
            {
                (this->*functions.at(choice - 1))();
            }
            else
            {
                Utils::printMessage("Opcao invalida. Tente novamente.");
            }
        }
    }
    catch (const exception &myException)
    {
        Utils::printMessage("Problema inesperado ao lancar acoes. " + string(myException.what()));
    }
}

void Controller::newWallet()
{
    Utils::printMessage("Incluir Nova Carteira");
    string holderName, exchangeName;

    cout << "Nome do Titular: ";
    getline(cin >> ws, holderName);

    cout << "Nome da Corretora: ";
    getline(cin >> ws, exchangeName);

    try
    {
        if (holderName.empty() || exchangeName.empty())
        {
            Utils::printMessage("Nome do titular ou corretora nao podem ser vazios. Operacao cancelada.");
            return;
        }
        WalletDTO *newWallet = new WalletDTO(Utils::AUTO_GENERATED_ID, holderName, exchangeName);
        walletDAO->addWallet(newWallet);
        Utils::printMessage("Carteira adicionada com sucesso!");
    }
    catch (const invalid_argument &e)
    {
        Utils::printMessage("Erro ao adicionar carteira: " + string(e.what()));
    }
    catch (const exception &e)
    {
        Utils::printMessage("Erro inesperado ao adicionar carteira: " + string(e.what()));
    }
}

void Controller::retrieveWallet()
{
    Utils::printMessage("Recuperar Carteira por ID");
    int walletId;
    cout << "Digite o ID da Carteira: ";

    while (!(cin >> walletId) || walletId <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "ID invalido. Digite um numero inteiro positivo: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    WalletDTO *wallet = walletDAO->getWalletById(walletId);

    if (wallet)
    {
        Utils::printMessage("Carteira Encontrada:");
        cout << "ID: " << wallet->getWalletId() << endl;
        cout << "Titular: " << wallet->getHolderName() << endl;
        cout << "Corretora: " << wallet->getExchangeName() << endl;

        delete wallet;
    }
    else
    {
        Utils::printMessage("Carteira com ID " + to_string(walletId) + " nao encontrada.");
    }
}

void Controller::editWallet()
{
    Utils::printMessage("Editar Carteira");
    int walletId;
    cout << "Digite o ID da Carteira a ser editada: ";

    while (!(cin >> walletId) || walletId <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "ID invalido. Digite um numero inteiro positivo: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    WalletDTO *existingWallet = walletDAO->getWalletById(walletId);
    if (existingWallet)
    {
        cout << "Dados Atuais:" << endl;
        cout << "ID: " << existingWallet->getWalletId() << endl;
        cout << "Titular: " << existingWallet->getHolderName() << endl;
        cout << "Corretora: " << existingWallet->getExchangeName() << endl;

        string newHolderName, newExchangeName;
        cout << "Novo Nome do Titular (<Enter> para manter " << existingWallet->getHolderName() << "): ";
        getline(cin, newHolderName);
        cout << "Nova Corretora (<Enter> para manter " << existingWallet->getExchangeName() << "): ";
        getline(cin, newExchangeName);

        string finalHolderName = newHolderName.empty() ? existingWallet->getHolderName() : newHolderName;
        string finalExchangeName = newExchangeName.empty() ? existingWallet->getExchangeName() : newExchangeName;

        try
        {
            WalletDTO *updatedWallet = new WalletDTO(walletId, finalHolderName, finalExchangeName);
            walletDAO->updateWallet(updatedWallet);
            Utils::printMessage("Carteira editada com sucesso!");
        }
        catch (const invalid_argument &e)
        {
            Utils::printMessage("Erro ao editar carteira: " + string(e.what()));
        }
        catch (const exception &e)
        {
            Utils::printMessage("Erro inesperado ao editar carteira: " + string(e.what()));
        }
    }
    else
    {
        Utils::printMessage("Carteira com ID " + to_string(walletId) + " nao encontrada.");
    }
}

void Controller::deleteWallet()
{
    Utils::printMessage("Excluir Carteira");
    int walletId;
    cout << "Digite o ID da Carteira a ser excluida: ";

    while (!(cin >> walletId) || walletId <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "ID invalido. Digite um numero inteiro positivo: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    WalletDTO *walletToDelete = walletDAO->getWalletById(walletId);
    if (walletToDelete)
    {
        cout << "Dados da Carteira a ser excluida:" << endl;
        cout << "ID: " << walletToDelete->getWalletId() << endl;
        cout << "Titular: " << walletToDelete->getHolderName() << endl;
        cout << "Corretora: " << walletToDelete->getExchangeName() << endl;

        string confirmation;
        cout << "Tem certeza que deseja excluir esta carteira? (S/N): ";
        getline(cin, confirmation);

        if (toupper(confirmation.empty() ? 'N' : confirmation.at(0)) == 'S')
        {
            walletDAO->deleteWallet(walletId);
            Utils::printMessage("Carteira excluida com sucesso!");
        }
        else
        {
            Utils::printMessage("Exclusao cancelada.");
        }
    }
    else
    {
        Utils::printMessage("Carteira com ID " + to_string(walletId) + " nao encontrada.");
    }
}

void Controller::listWalletsById()
{
    Utils::printMessage("Listar Carteiras por ID");
    vector<WalletDTO *> wallets = walletDAO->getAllWallets();

    if (wallets.empty())
    {
        Utils::printMessage("Nenhuma carteira registrada.");
        return;
    }

    sort(wallets.begin(), wallets.end(), [](WalletDTO *a, WalletDTO *b)
         { return a->getWalletId() < b->getWalletId(); });

    string decorator = Utils::replicate(Utils::DECORATOR_EQUALS, Utils::TABLE_WIDTH_NORMAL);
    cout << decorator << endl;
    cout << left << setw(10) << "ID" << setw(30) << "Titular" << setw(30) << "Corretora" << endl;
    cout << decorator << endl;

    for (WalletDTO *wallet : wallets)
    {
        cout << left << setw(10) << wallet->getWalletId()
             << setw(30) << wallet->getHolderName()
             << setw(30) << wallet->getExchangeName() << endl;
    }
    cout << decorator << endl;
}

void Controller::listWalletsByHolderName()
{
    Utils::printMessage("Listar Carteiras por Nome do Titular");
    vector<WalletDTO *> wallets = walletDAO->getAllWallets();

    if (wallets.empty())
    {
        Utils::printMessage("Nenhuma carteira registrada.");
        return;
    }

    sort(wallets.begin(), wallets.end(), [](WalletDTO *a, WalletDTO *b)
         { return a->getHolderName() < b->getHolderName(); });

    string decorator = Utils::replicate(Utils::DECORATOR_EQUALS, Utils::TABLE_WIDTH_NORMAL);
    cout << decorator << endl;
    cout << left << setw(10) << "ID" << setw(30) << "Titular" << setw(30) << "Corretora" << endl;
    cout << decorator << endl;

    for (WalletDTO *wallet : wallets)
    {
        cout << left << setw(10) << wallet->getWalletId()
             << setw(30) << wallet->getHolderName()
             << setw(30) << wallet->getExchangeName() << endl;
    }
    cout << decorator << endl;
}

void Controller::registerPurchase()
{
    Utils::printMessage("Registrar Compra de Moeda");
    int walletId;
    cout << "ID da Carteira: ";
    while (!(cin >> walletId) || walletId <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "ID invalido. Digite um numero inteiro positivo: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    WalletDTO *targetWallet = walletDAO->getWalletById(walletId);
    if (!targetWallet)
    {
        Utils::printMessage("Carteira com ID " + to_string(walletId) + " nao encontrada. Operacao cancelada.");
        return;
    }
    delete targetWallet; // Liberar memória, pois só precisamos do ID por enquanto

    Date opDate;
    cout << "Data da Operacao (DD.MM.YYYY): ";
    while (!(cin >> opDate))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Data invalida. Digite no formato DD.MM.YYYY: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    OracleDTO *quote = oracleDAO->getQuoteByDate(opDate);
    if (quote == nullptr)
    {
        Utils::printMessage(string(Utils::ANSI_RED) + "ERRO: Nao existe cotacao para a data " + opDate.getIsoFormat() + ". Operacao cancelada." + Utils::ANSI_RESET);
        return;
    }
    delete quote;

    double quantity;
    cout << "Quantidade Comprada: ";
    while (!(cin >> quantity) || quantity <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Quantidade invalida. Digite um valor numerico positivo: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    try
    {
        MovementDTO *newMovement = new MovementDTO(Utils::AUTO_GENERATED_ID, walletId, opDate, OperationType::BUY, quantity);
        movementDAO->registerTransaction(newMovement);
        Utils::printMessage("Compra registrada com sucesso na carteira " + to_string(walletId) + "!");
    }
    catch (const invalid_argument &e)
    {
        Utils::printMessage("Erro ao registrar compra: " + string(e.what()));
    }
    catch (const exception &e)
    {
        Utils::printMessage("Erro inesperado ao registrar compra: " + string(e.what()));
    }
}

void Controller::registerSale()
{
    Utils::printMessage("Registrar Venda de Moeda");
    int walletId;
    cout << "ID da Carteira: ";
    while (!(cin >> walletId) || walletId <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "ID invalido. Digite um numero inteiro positivo: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    WalletDTO *targetWallet = walletDAO->getWalletById(walletId);
    if (!targetWallet)
    {
        Utils::printMessage("Carteira com ID " + to_string(walletId) + " nao encontrada. Operacao cancelada.");
        return;
    }
    delete targetWallet; // Liberar memória

    Date opDate;
    cout << "Data da Operacao (DD.MM.YYYY): ";
    while (!(cin >> opDate))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Data invalida. Digite no formato DD.MM.YYYY: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    OracleDTO *quote = oracleDAO->getQuoteByDate(opDate);
    if (quote == nullptr)
    {
        Utils::printMessage(string(Utils::ANSI_RED) + "ERRO: Nao existe cotacao para a data " + opDate.getIsoFormat() + ". Operacao cancelada." + Utils::ANSI_RESET);
        return; // Interrompe a função aqui se a cotação não for encontrada
    }
    delete quote;

    double quantity;
    cout << "Quantidade Vendida: ";
    while (!(cin >> quantity) || quantity <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Quantidade invalida. Digite um valor numerico positivo: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    try
    {
        MovementDTO *newMovement = new MovementDTO(Utils::AUTO_GENERATED_ID, walletId, opDate, OperationType::SELL, quantity);
        movementDAO->registerTransaction(newMovement);
        Utils::printMessage("Venda registrada com sucesso na carteira " + to_string(walletId) + "!");
    }
    catch (const invalid_argument &e)
    {
        Utils::printMessage("Erro ao registrar venda: " + string(e.what()));
    }
    catch (const exception &e)
    {
        Utils::printMessage("Erro inesperado ao registrar venda: " + string(e.what()));
    }
}

void Controller::showWalletBalance()
{
    Utils::printMessage("Exibir Saldo da Carteira");
    int walletId;
    cout << "Digite o ID da Carteira: ";
    while (!(cin >> walletId) || walletId <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "ID invalido. Digite um numero inteiro positivo: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    WalletDTO *targetWallet = walletDAO->getWalletById(walletId);
    if (!targetWallet)
    {
        Utils::printMessage("Carteira com ID " + to_string(walletId) + " nao encontrada.");
        return;
    }

    double saldo = businessLogic->calculateWalletBalance(walletId);

    cout << Utils::replicate(Utils::DECORATOR_EQUALS, Utils::TABLE_WIDTH_NORMAL) << endl;
    cout << "Saldo da Carteira " << targetWallet->getHolderName()
         << " (ID: " << targetWallet->getWalletId() << "): R$ "
         << fixed << setprecision(2) << saldo << endl;
    cout << Utils::replicate(Utils::DECORATOR_EQUALS, Utils::TABLE_WIDTH_NORMAL) << endl;

    delete targetWallet;
}

void Controller::showMovementHistory()
{
    Utils::printMessage("Exibir Historico de Movimentacao da Carteira");

    int walletId;
    cout << "Digite o ID da Carteira: ";
    while (!(cin >> walletId) || walletId <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "ID invalido. Digite um numero inteiro positivo: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    WalletDTO *targetWallet = walletDAO->getWalletById(walletId);
    if (!targetWallet)
    {
        Utils::printMessage("Carteira com ID " + to_string(walletId) + " nao encontrada.");
        return;
    }

    businessLogic->detailedWalletReport(walletId);

    delete targetWallet;
}

void Controller::showGainsLosses()
{
    Utils::printMessage("Exibir Ganhos e Perdas por Carteira");

    vector<WalletDTO *> allWallets = walletDAO->getAllWallets();
    if (allWallets.empty())
    {
        Utils::printMessage("Nenhuma carteira registrada para calcular ganhos/perdas.");
        return;
    }

    cout << Utils::replicate(Utils::DECORATOR_EQUALS, Utils::TABLE_WIDTH_WIDE) << endl;
    cout << left << setw(10) << "ID" << setw(30) << "Titular" << setw(30) << "Corretora" << setw(20) << "Ganho/Perda (R$)" << endl;
    cout << Utils::replicate(Utils::DECORATOR_EQUALS, Utils::TABLE_WIDTH_WIDE) << endl;

    for (WalletDTO *wallet : allWallets)
    {
        double gainLoss = businessLogic->calculateGainLoss(wallet->getWalletId());

        string color = (gainLoss >= 0) ? Utils::ANSI_GREEN : Utils::ANSI_RED;

        cout << left << setw(10) << wallet->getWalletId()
             << setw(30) << wallet->getHolderName()
             << setw(30) << wallet->getExchangeName()
             << color << fixed << setprecision(2) << setw(20) << gainLoss << Utils::ANSI_RESET << endl;

        delete wallet;
    }

    cout << Utils::replicate(Utils::DECORATOR_EQUALS, Utils::TABLE_WIDTH_WIDE) << endl;
}

void Controller::showHelpText()
{
    Utils::printMessage("Texto de Ajuda do FT_Coin");
    unique_ptr<TextFromFile> textFromFile(new TextFromFile("help.txt"));
    Utils::printFramedMessage(textFromFile->getFileContent(), Utils::DECORATOR_DASH, Utils::TABLE_WIDTH_WIDE);
}

void Controller::showCredits()
{
    string text = "";
    text += SysInfo::getFullVersion() + "\n";
    text += "Desenvolvido por: Andrezza, Bianca, Henrique, Sidinei, Yasmin\n";
    text += SysInfo::getInstitution() + "\n";
    text += SysInfo::getDepartment() + "\n";
    text += "Copyright " + SysInfo::getAuthor() + " " + SysInfo::getDate() + "\n";
    Utils::printMessage(SysInfo::getVersion() + " | Creditos");
    Utils::printFramedMessage(text, Utils::DECORATOR_EQUALS, Utils::TABLE_WIDTH_WIDE);
}

void Controller::populateDemoData()
{

#ifdef USE_MARIADB
    Utils::printMessage("Populando dados de demonstracao (MariaDB)... ");

    try
    {
        if (serverDBConnection)
        {
            serverDBConnection->getConnection()->setAutoCommit(false);
        }

        movementDAO->clearAll();
        walletDAO->clearAll();
        oracleDAO->clearAll();

        WalletDTO *w1 = new WalletDTO(Utils::AUTO_GENERATED_ID, "Maria Silva", "Binance");
        walletDAO->addWallet(w1);
        WalletDTO *w2 = new WalletDTO(Utils::AUTO_GENERATED_ID, "João Pereira", "Coinbase");
        walletDAO->addWallet(w2);
        WalletDTO *w3 = new WalletDTO(Utils::AUTO_GENERATED_ID, "Ana Souza", "Mercado Bitcoin");
        walletDAO->addWallet(w3);

        int mariaId = walletDAO->getAllWallets().at(0)->getWalletId();
        int joaoId = walletDAO->getAllWallets().at(1)->getWalletId();
        int anaId = walletDAO->getAllWallets().at(2)->getWalletId();

        oracleDAO->saveQuote(new OracleDTO(Date(10, 5, 2024), 9500.00));
        oracleDAO->saveQuote(new OracleDTO(Date(11, 5, 2024), 9000.00));
        oracleDAO->saveQuote(new OracleDTO(Date(12, 5, 2024), 9800.00));
        oracleDAO->saveQuote(new OracleDTO(Date(13, 5, 2024), 9100.00));
        oracleDAO->saveQuote(new OracleDTO(Date(14, 5, 2024), 8700.00));
        oracleDAO->saveQuote(new OracleDTO(Date(15, 5, 2024), 9200.00));
        oracleDAO->saveQuote(new OracleDTO(Date(16, 5, 2024), 9900.00));
        oracleDAO->saveQuote(new OracleDTO(Date(17, 5, 2024), 9500.00));

        movementDAO->registerTransaction(new MovementDTO(Utils::AUTO_GENERATED_ID, mariaId, Date(10, 5, 2024), OperationType::BUY, 1.5));
        movementDAO->registerTransaction(new MovementDTO(Utils::AUTO_GENERATED_ID, mariaId, Date(13, 5, 2024), OperationType::SELL, 0.5));
        movementDAO->registerTransaction(new MovementDTO(Utils::AUTO_GENERATED_ID, joaoId, Date(11, 5, 2024), OperationType::BUY, 2.0));
        movementDAO->registerTransaction(new MovementDTO(Utils::AUTO_GENERATED_ID, joaoId, Date(17, 5, 2024), OperationType::SELL, 1.0));
        movementDAO->registerTransaction(new MovementDTO(Utils::AUTO_GENERATED_ID, anaId, Date(12, 5, 2024), OperationType::BUY, 0.8));
        movementDAO->registerTransaction(new MovementDTO(Utils::AUTO_GENERATED_ID, anaId, Date(16, 5, 2024), OperationType::SELL, 0.3));

        if (serverDBConnection)
        {
            serverDBConnection->getConnection()->commit();
            serverDBConnection->getConnection()->setAutoCommit(true);
        }

        Utils::printMessage("Dados de demonstracao populados com sucesso (MariaDB)!");
    }
    catch (const sql::SQLException &e)
    {
        Utils::printMessage("ERRO ao popular dados de demonstracao no MariaDB: " + string(e.what()));
        if (serverDBConnection)
        {
            try
            {
                serverDBConnection->getConnection()->rollback();
                serverDBConnection->getConnection()->setAutoCommit(true);
            }
            catch (const sql::SQLException &rb_e)
            {
                cerr << "Erro no rollback durante populacao: " << rb_e.what() << endl;
            }
        }
    }
    catch (const exception &e)
    {
        Utils::printMessage("ERRO inesperado ao popular dados de demonstracao: " + string(e.what()));
        if (serverDBConnection)
        {
            try
            {
                serverDBConnection->getConnection()->rollback();
                serverDBConnection->getConnection()->setAutoCommit(true);
            }
            catch (const sql::SQLException &rb_e)
            {
                cerr << "Erro no rollback durante populacao: " << rb_e.what() << endl;
            }
        }
    }
#else

    Utils::printMessage("Populando dados de demonstracao (MEMORIA)...");

    movementDAO->clearAll();
    walletDAO->clearAll();
    oracleDAO->clearAll();

    WalletDTO *w1 = new WalletDTO(Utils::AUTO_GENERATED_ID, "Maria Silva", "Binance");
    walletDAO->addWallet(w1);
    WalletDTO *w2 = new WalletDTO(Utils::AUTO_GENERATED_ID, "João Pereira", "Coinbase");
    walletDAO->addWallet(w2);
    WalletDTO *w3 = new WalletDTO(Utils::AUTO_GENERATED_ID, "Ana Souza", "Mercado Bitcoin");
    walletDAO->addWallet(w3);

    int mariaId = walletDAO->getAllWallets().at(0)->getWalletId();
    int joaoId = walletDAO->getAllWallets().at(1)->getWalletId();
    int anaId = walletDAO->getAllWallets().at(2)->getWalletId();

    oracleDAO->saveQuote(new OracleDTO(Date(10, 5, 2024), 9500.00));
    oracleDAO->saveQuote(new OracleDTO(Date(11, 5, 2024), 9000.00));
    oracleDAO->saveQuote(new OracleDTO(Date(12, 5, 2024), 9800.00));
    oracleDAO->saveQuote(new OracleDTO(Date(13, 5, 2024), 9100.00));
    oracleDAO->saveQuote(new OracleDTO(Date(14, 5, 2024), 8700.00));
    oracleDAO->saveQuote(new OracleDTO(Date(15, 5, 2024), 9200.00));
    oracleDAO->saveQuote(new OracleDTO(Date(16, 5, 2024), 9900.00));
    oracleDAO->saveQuote(new OracleDTO(Date(17, 5, 2024), 9500.00));

    movementDAO->registerTransaction(new MovementDTO(Utils::AUTO_GENERATED_ID, mariaId, Date(10, 5, 2024), OperationType::BUY, 1.5));
    movementDAO->registerTransaction(new MovementDTO(Utils::AUTO_GENERATED_ID, mariaId, Date(13, 5, 2024), OperationType::SELL, 0.5));
    movementDAO->registerTransaction(new MovementDTO(Utils::AUTO_GENERATED_ID, joaoId, Date(11, 5, 2024), OperationType::BUY, 2.0));
    movementDAO->registerTransaction(new MovementDTO(Utils::AUTO_GENERATED_ID, joaoId, Date(17, 5, 2024), OperationType::SELL, 1.0));
    movementDAO->registerTransaction(new MovementDTO(Utils::AUTO_GENERATED_ID, anaId, Date(12, 5, 2024), OperationType::BUY, 0.8));
    movementDAO->registerTransaction(new MovementDTO(Utils::AUTO_GENERATED_ID, anaId, Date(16, 5, 2024), OperationType::SELL, 0.3));

    Utils::printMessage("Dados de demonstracao populados com sucesso (MEMORIA)!");
#endif
}

void Controller::exportHistoryToCSV()
{
    Utils::printMessage("Exportar Historico para CSV");
    int walletId;
    cout << "Digite o ID da Carteira a ser exportada: ";

    while (!(cin >> walletId) || walletId <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "ID invalido. Digite um numero inteiro positivo: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    WalletDTO *wallet = walletDAO->getWalletById(walletId);
    if (!wallet)
    {
        Utils::printMessage("Carteira com ID " + to_string(walletId) + " nao encontrada.");
        return;
    }

    vector<MovementDTO *> history = movementDAO->getHistoryByWalletId(walletId);

    string fileName = "historico_carteira_" + to_string(walletId) + ".csv";
    ofstream csvFile;
    csvFile.open(fileName);

    if (!csvFile.is_open())
    {
        Utils::printMessage("ERRO: Nao foi possivel criar o arquivo " + fileName);
        delete wallet;
        for (MovementDTO *mov : history)
        {
            delete mov;
        }
        return;
    }

    csvFile << "Data,Tipo,Quantidade,CotacaoNaData,ValorTotal\n";

    for (MovementDTO *mov : history)
    {
        OracleDTO *quoteObj = oracleDAO->getQuoteByDate(mov->getDate());
        double quoteValue = quoteObj ? quoteObj->getQuote() : 0.0;
        double totalValue = mov->getQuantity() * quoteValue;
        string typeStr = (mov->getOperationType() == OperationType::BUY) ? "Compra" : "Venda";

        csvFile << mov->getDate().getIsoFormat() << ","
                << typeStr << ","
                << mov->getQuantity() << ","
                << quoteValue << ","
                << totalValue << "\n";

        if (quoteObj)
        {
            delete quoteObj;
        }
    }

    csvFile.close();
    Utils::printMessage("Arquivo '" + fileName + "' gerado com sucesso!");

    delete wallet;
    for (MovementDTO *mov : history)
    {
        delete mov;
    }
}