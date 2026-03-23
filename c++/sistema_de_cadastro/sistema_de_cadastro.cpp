#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Conta {
    int id;
    string senha;
};

vector<Conta> contas; // "Banco de dados" em memória
int proximoId = 1;    // Gerador de IDs

// Cadastro de conta
void cadastrar() {
    string senha;
    cout << "Crie sua senha: ";
    cin >> senha;

    Conta novaConta{proximoId++, senha};
    contas.push_back(novaConta);

    cout << "Conta criada com sucesso! Seu ID é: " << novaConta.id << endl;
}

// Login
void login() {
    int id;
    string senha;
    cout << "Digite seu ID: ";
    cin >> id;
    cout << "Digite sua senha: ";
    cin >> senha;

    bool encontrado = false;
    for (auto& conta : contas) {
        if (conta.id == id) {
            encontrado = true;
            if (conta.senha == senha) {
                cout << "Login realizado com sucesso!" << endl;
            } else {
                cout << "Senha incorreta." << endl;
            }
            break;
        }
    }
    if (!encontrado) {
        cout << "ID não encontrado." << endl;
    }
}

// Mostrar apenas IDs cadastrados
void mostrarContas() {
    cout << "\n--- IDs cadastrados ---" << endl;
    for (auto& conta : contas) {
        cout << "ID: " << conta.id << endl;
    }
    cout << "-----------------------\n" << endl;
}

// Menu principal
void menu() {
    cout << "--- MENU ---" << endl;
    cout << "1 - Fazer login" << endl;
    cout << "2 - Fazer cadastro" << endl;
    cout << "3 - Mostrar contas" << endl;
    cout << "0 - Sair" << endl;
    cout << "Escolha uma opção: ";
}

// Função principal
int main() {
    int op;
    do {
        menu();
        cin >> op;
        switch(op) {
            case 1: login(); break;
            case 2: cadastrar(); break;
            case 3: mostrarContas(); break;
            case 0: cout << "Saindo..." << endl; break;
            default: cout << "Opção inválida." << endl; break;
        }
    } while(op != 0);

    return 0;
}

