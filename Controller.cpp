#include "Controller.hpp"
#include <cctype>    // Para toupper
#include <iomanip>   // Para formatação de saída
#include <limits>    // Para numeric_limits
#include <stdexcept> // Para exceções
#include <algorithm> // Para std::sort e std::max

// Inclui as implementações concretas dos DAOs para instanciar
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

// Usar std:: para evitar 'using namespace std;' em .cpp se preferir
// Alternativamente, para simplificar neste arquivo:
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
            }
        catch (const exception& e) 
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
    const string ANSI_RED = "\033[0;31m";
    const string ANSI_RESET = "\033[0m";

vector<string> mainMenuItems = {"Carteira", "Movimentacao", "Relatorios", "Ajuda", ANSI_RED + "Sair do Programa" + ANSI_RESET};    vector<void (Controller::*)()> functions = {
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
    vector<string> menuItens = {"Exibir Saldo da Carteira", "Exibir Historico de Movimentacao", "Exibir Ganhos/Perdas", "Voltar"};
    vector<void (Controller::*)()> functions = {
        &Controller::showWalletBalance,
        &Controller::showMovementHistory,
        &Controller::showGainsLosses};
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
    // Implementação da tela de créditos no showCredits()
    showCredits();
}

void Controller::launchActions(string title, vector<string> menuItens, vector<void (Controller::*)()> functions)
{
    try
    {
        Menu menu(menuItens, title, "Sua opcao: ");
        menu.setSymbol("*");
        menu.setZeroForLastOpt(true); // "Voltar" geralmente é a última opção e pode ser 0

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

// *** Implementações das funcionalidades da Carteira ***
void Controller::newWallet()
{
    Utils::printMessage("Incluir Nova Carteira");
    string holderName, exchangeName;

    cout << "Nome do Titular: ";
    getline(cin >> ws, holderName); // ws para consumir nova linha pendente

    cout << "Nome da Corretora: ";
    getline(cin >> ws, exchangeName);

    try
    {
        if (holderName.empty() || exchangeName.empty())
        {
            Utils::printMessage("Nome do titular ou corretora nao podem ser vazios. Operacao cancelada.");
            return;
        }
        WalletDTO *newWallet = new WalletDTO(0, holderName, exchangeName); // ID 0 para ser definido pelo DAO
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
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpa buffer

    // 1. Chama o método do DAO para obter um ponteiro para uma NOVA cópia do objeto.
    WalletDTO *wallet = walletDAO->getWalletById(walletId);

    // 2. Verifica se a carteira foi encontrada.
    if (wallet)
    {
        Utils::printMessage("Carteira Encontrada:");
        cout << "ID: " << wallet->getWalletId() << endl;
        cout << "Titular: " << wallet->getHolderName() << endl;
        cout << "Corretora: " << wallet->getExchangeName() << endl;

        // 3. IMPORTANTE: Libera a memória da cópia que foi criada.
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

        // Criar um novo DTO com os dados atualizados
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

    // Ordenar por ID (já deve ser o padrão se o DAO for bem feito, mas podemos garantir)
    sort(wallets.begin(), wallets.end(), [](WalletDTO *a, WalletDTO *b)
         { return a->getWalletId() < b->getWalletId(); });

    string decorator = Utils::replicate("=", 80);
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

    // Ordenar por Nome do Titular
    sort(wallets.begin(), wallets.end(), [](WalletDTO *a, WalletDTO *b)
         { return a->getHolderName() < b->getHolderName(); });

    string decorator = Utils::replicate("=", 80);
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

// *** Implementações das funcionalidades de Movimentação ***
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

    Date opDate;
    cout << "Data da Operacao (DD.MM.YYYY): ";
    while (!(cin >> opDate))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Data invalida. Digite no formato DD.MM.YYYY: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    double quantity;
    cout << "Quantidade Comprada: ";
    while (!(cin >> quantity) || quantity <= 0)
    { // Compra deve ter quantidade positiva
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Quantidade invalida. Digite um valor numerico positivo: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    try
    {
        MovementDTO *newMovement = new MovementDTO(0, walletId, opDate, 'C', quantity);
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

    Date opDate;
    cout << "Data da Operacao (DD.MM.YYYY): ";
    while (!(cin >> opDate))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Data invalida. Digite no formato DD.MM.YYYY: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    double quantity;
    cout << "Quantidade Vendida: ";
    while (!(cin >> quantity) || quantity <= 0)
    { // Venda deve ter quantidade positiva
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Quantidade invalida. Digite um valor numerico positivo: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    try
    {
        MovementDTO *newMovement = new MovementDTO(0, walletId, opDate, 'V', quantity);
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

// *** Implementações das funcionalidades de Relatórios ***
void Controller::showWalletBalance()
{
    Utils::printMessage("Exibir Saldo Atual da Carteira");
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

    cout << Utils::replicate("=", 80) << endl;
    cout << "Saldo da Carteira " << targetWallet->getHolderName()
         << " (ID: " << targetWallet->getWalletId() << "): R$ "
         << fixed << setprecision(2) << saldo << endl;
    cout << Utils::replicate("=", 80) << endl;

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

    //Usa a lógica de negócio para gerar o relatório
    businessLogic->detailedWalletReport(walletId);

    delete targetWallet; //liberar memória
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

    cout << Utils::replicate("=", 100) << endl;
    cout << left << setw(10) << "ID" << setw(30) << "Titular" << setw(30) << "Corretora" << setw(20) << "Ganho/Perda (R$)" << endl;
    cout << Utils::replicate("=", 100) << endl;

    for (WalletDTO *wallet : allWallets)
    {
        double gainLoss = businessLogic->calculateGainLoss(wallet->getWalletId());

        const string ANSI_RED = "\033[0;31m";
        const string ANSI_GREEN = "\033[1;32m";
        const string ANSI_RESET = "\033[0m";

        string color = (gainLoss >= 0) ? ANSI_GREEN : ANSI_RED;

        cout << left << setw(10) << wallet->getWalletId()
             << setw(30) << wallet->getHolderName()
             << setw(30) << wallet->getExchangeName()
             << color << fixed << setprecision(2) << setw(20) << gainLoss << ANSI_RESET << endl;

        delete wallet; // liberar memória
    }

    cout << Utils::replicate("=", 100) << endl;
}

void Controller::showHelpText()
{
    Utils::printMessage("Texto de Ajuda do FT_Coin");
    // Confirmar que "help.txt" existe e está no diretório correto
    unique_ptr<TextFromFile> textFromFile(new TextFromFile("help.txt"));
    Utils::printFramedMessage(textFromFile->getFileContent(), "-", 100);
}

void Controller::showCredits()
{
    string text = "";
    text += SysInfo::getFullVersion() + "\n";
    text += "Desenvolvido por: Andrezza,, Henrique, Sidinei, Yasmin\n"; // Adapte aqui com os nomes dos integrantes
    text += SysInfo::getInstitution() + "\n";
    text += SysInfo::getDepartment() + "\n";
    text += "Copyright " + SysInfo::getAuthor() + " " + SysInfo::getDate() + "\n";
    Utils::printMessage(SysInfo::getVersion() + " | Creditos");
    Utils::printFramedMessage(text, "=", 100);
}

// *** Dados de Demonstração ***
void Controller::populateDemoData()
{
    Utils::printMessage("Populando dados de demonstracao (apenas para DB em memoria)...");

    //Carteiras
    WalletDTO *w1 = new WalletDTO(0, "Maria Silva", "Binance");
    walletDAO->addWallet(w1);
    WalletDTO *w2 = new WalletDTO(0, "João Pereira", "Coinbase");
    walletDAO->addWallet(w2);
    WalletDTO *w3 = new WalletDTO(0, "Ana Souza", "Mercado Bitcoin");
    walletDAO->addWallet(w3);

    // Assegura que os IDs foram setados pelo DAO em memória
    int mariaId = walletDAO->getAllWallets().at(0)->getWalletId();
    int joaoId = walletDAO->getAllWallets().at(1)->getWalletId();
    int anaId = walletDAO->getAllWallets().at(2)->getWalletId();

    //Oraculo (Cotações)
    oracleDAO->saveQuote(new OracleDTO(Date(10, 5, 2024), 10000.00));
    oracleDAO->saveQuote(new OracleDTO(Date(11, 5, 2024), 10500.00));
    oracleDAO->saveQuote(new OracleDTO(Date(12, 5, 2024), 9800.00));
    oracleDAO->saveQuote(new OracleDTO(Date(13, 5, 2024), 11000.00));
    oracleDAO->saveQuote(new OracleDTO(Date(14, 5, 2024), 10200.00));
    oracleDAO->saveQuote(new OracleDTO(Date(15, 5, 2024), 10800.00));
    oracleDAO->saveQuote(new OracleDTO(Date(16, 5, 2024), 11500.00)); // Cotação alta
    oracleDAO->saveQuote(new OracleDTO(Date(17, 5, 2024), 9500.00));  // Cotação baixa

    // Movimentações
    // Maria: Compra 1.5 em 10/05/2024 (1.5 * 10000 = 15000)
    movementDAO->registerTransaction(new MovementDTO(0, mariaId, Date(10, 5, 2024), 'C', 1.5));
    // Maria: Vende 0.5 em 13/05/2024 (0.5 * 11000 = 5500)
    movementDAO->registerTransaction(new MovementDTO(0, mariaId, Date(13, 5, 2024), 'V', 0.5));
    // João: Compra 2.0 em 11/05/2024 (2.0 * 10500 = 21000)
    movementDAO->registerTransaction(new MovementDTO(0, joaoId, Date(11, 5, 2024), 'C', 2.0));
    // João: Vende 1.0 em 17/05/2024 (1.0 * 9500 = 9500) -> provavel perda
    movementDAO->registerTransaction(new MovementDTO(0, joaoId, Date(17, 5, 2024), 'V', 1.0));
    // Ana: Compra 0.8 em 12/05/2024 (0.8 * 9800 = 7840)
    movementDAO->registerTransaction(new MovementDTO(0, anaId, Date(12, 5, 2024), 'C', 0.8));
    // Ana: Vende 0.3 em 16/05/2024 (0.3 * 11500 = 3450) -> provavel ganho
    movementDAO->registerTransaction(new MovementDTO(0, anaId, Date(16, 5, 2024), 'V', 0.3));

    Utils::printMessage("Dados de demonstracao populados com sucesso!");
}