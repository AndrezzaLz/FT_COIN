#include <iostream>
#include "CarteiraDTO.hpp"
#include "MovimentacaoDTO.hpp"
#include "OracleDTO.hpp"
#include "DAO_Memoria.hpp"
#include "Menu.h"

using namespace std;

int main() {
    try {
        vector<string> opcoesMenu = {
            "Cadastrar Carteira",
            "Cadastrar Movimentação",
            "Cadastrar Cotação",
            "Mostrar Movimentações",
            "Sair"
        };

        Menu menu(opcoesMenu, "Menu Principal", "Escolha uma opção:");
        menu.setRepeatList(true);
        menu.setZeroForLastOpt(true); // Mostra "0 - Sair"
        menu.setSymbol("=");

        // Objetos persistentes
        CarteiraDTO carteira = { 1, "Jefferson", "XP"};
        DAO_Memoria dao;

        int escolha;
        do {
            escolha = menu.getChoice();

            switch (escolha) {
                case 1: {
                    cout << "--- Cadastrar Carteira ---" << endl;
                    int idCarteira;
                    string nomeTitular, corretora;

                    cout << "ID da Carteira: ";
                    cin >> idCarteira;
                    cin.ignore();

                    cout << "Nome do Titular: ";
                    getline(cin, nomeTitular);

                    cout << "Nome da Corretora: ";
                    getline(cin, corretora);

                    carteira = CarteiraDTO(idCarteira, nomeTitular, corretora);
                    cout << "Carteira criada com sucesso!" << endl << endl;
                    break;
                }

                case 2: {
                    cout << "--- Cadastrar Movimentação ---" << endl;
                    int idMov;
                    string dataMov;
                    char tipo;
                    double quantidade;

                    cout << "ID da Movimentação: ";
                    cin >> idMov;

                    cout << "Data (YYYY-MM-DD): ";
                    cin >> dataMov;

                    cout << "Tipo ('C' para compra, 'V' para venda): ";
                    cin >> tipo;

                    cout << "Quantidade: ";
                    cin >> quantidade;

                    MovimentacaoDTO mov(idMov, carteira.id, dataMov, tipo, quantidade);
                    dao.create(mov);

                    cout << "Movimentação criada com sucesso!" << endl << endl;
                    break;
                }

                case 3: {
                    cout << "--- Cadastrar Cotação (Oracle) ---" << endl;
                    string dataCot;
                    double cotacao;

                    cout << "Data (YYYY-MM-DD): ";
                    cin >> dataCot;

                    cout << "Cotação da moeda: ";
                    cin >> cotacao;

                    OracleDTO oracle(dataCot, cotacao);
                    cout << "Cotação registrada com sucesso!" << endl << endl;
                    break;
                }

                case 4: {
                    cout << "--- Movimentações Armazenadas ---" << endl;
                    for (const auto& m : dao.listAll()) {
                        cout << "ID: " << m.id
                             << ", Carteira: " << m.idCarteira
                             << ", Data: " << m.data
                             << ", Tipo: " << m.tipoOperacao
                             << ", Quantidade: " << m.quantidade << endl;
                    }
                    cout << endl;
                    break;
                }

                case 0:
                    cout << "Encerrando o programa." << endl;
                    break;

                default:
                    cout << "Opção inválida!" << endl;
            }

        } while (escolha != 0);

    } catch (const exception& e) {
        cerr << "Erro: " << e.what() << endl;
    }

    return 0;
}
