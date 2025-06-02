#include <iostream>
#include "CarteiraDTO.hpp"
#include "MovimentacaoDTO.hpp"
#include "OracleDTO.hpp"
#include "DAO_Memoria.hpp"

using namespace std;

int main() {
    try {
        cout << "--- Teste DTO: Carteira ---" << endl;
        int idCarteira;
        string nomeTitular, corretora;

        cout << "ID da Carteira: ";
        cin >> idCarteira;
        cin.ignore();

        cout << "Nome do Titular: ";
        getline(cin, nomeTitular);

        cout << "Nome da Corretora: ";
        getline(cin, corretora);

        CarteiraDTO carteira(idCarteira, nomeTitular, corretora);
        cout << "Carteira criada com sucesso!" << endl << endl;

        cout << "--- Teste DTO: Movimentação ---" << endl;
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

        MovimentacaoDTO mov(idMov, idCarteira, dataMov, tipo, quantidade);
        cout << "Movimentação criada com sucesso!" << endl << endl;

        cout << "--- Teste DTO: Oracle (Cotação) ---" << endl;
        string dataCot;
        double cotacao;

        cout << "Data (YYYY-MM-DD): ";
        cin >> dataCot;

        cout << "Cotação da moeda: ";
        cin >> cotacao;

        OracleDTO oracle(dataCot, cotacao);
        cout << "Cotação registrada com sucesso!" << endl << endl;

        cout << "--- Teste DAO em Memória ---" << endl;
        DAO_Memoria dao;
        dao.create(mov);  // adiciona a movimentação criada

        cout << "Movimentações armazenadas:" << endl;
        for (const auto& m : dao.listAll()) {
            cout << "ID: " << m.id << ", Carteira: " << m.idCarteira
                 << ", Data: " << m.data << ", Tipo: " << m.tipoOperacao
                 << ", Quantidade: " << m.quantidade << endl;
        }

    } catch (const exception& e) {
        cerr << "Erro ao criar DTO ou usar DAO: " << e.what() << endl;
    }

    return 0;
}
