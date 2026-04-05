#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

// Arquivo onde as senhas ficam salvas
const char* ARQUIVO = "senhas.bin";

// Chave simples para criptografia XOR
const char CHAVE = 'K';

// Estrutura do registro
struct Registro {
    char servico[50];
    char senha[50];
    bool deletado;
};

// Função de criptografia / descriptografia (XOR)
void criptografar(char* texto) {
    for (int i = 0; texto[i] != '\0'; i++) {
        texto[i] ^= CHAVE;
    }
}

// Adicionar senha
void adicionar() {
    Registro r;
    cout << "Servico: ";
    cin >> r.servico;

    cout << "Senha: ";
    cin >> r.senha;

    criptografar(r.senha);
    r.deletado = false;

    ofstream arquivo(ARQUIVO, ios::binary | ios::app);
    arquivo.write((char*)&r, sizeof(Registro));
    arquivo.close();

    cout << "Senha adicionada com sucesso.\n";
}

// Buscar senha
void buscar() {
    char servicoBusca[50];
    cout << "Servico: ";
    cin >> servicoBusca;

    ifstream arquivo(ARQUIVO, ios::binary);
    Registro r;

    while (arquivo.read((char*)&r, sizeof(Registro))) {
        if (!r.deletado && strcmp(r.servico, servicoBusca) == 0) {
            criptografar(r.senha); // descriptografa
            cout << "Senha: " << r.senha << endl;
            arquivo.close();
            return;
        }
    }

    cout << "Servico nao encontrado.\n";
    arquivo.close();
}

// Listar serviços
void listar() {
    ifstream arquivo(ARQUIVO, ios::binary);
    Registro r;

    cout << "Servicos salvos:\n";

    while (arquivo.read((char*)&r, sizeof(Registro))) {
        if (!r.deletado) {
            cout << "- " << r.servico << endl;
        }
    }

    arquivo.close();
}

// Deletar senha (logico)
void deletar() {
    char servicoBusca[50];
    cout << "Servico: ";
    cin >> servicoBusca;

    fstream arquivo(ARQUIVO, ios::binary | ios::in | ios::out);
    Registro r;

    while (arquivo.read((char*)&r, sizeof(Registro))) {
        if (!r.deletado && strcmp(r.servico, servicoBusca) == 0) {
            r.deletado = true;
            arquivo.seekp(-sizeof(Registro), ios::cur);
            arquivo.write((char*)&r, sizeof(Registro));
            cout << "Senha removida.\n";
            arquivo.close();
            return;
        }
    }

    cout << "Servico nao encontrado.\n";
    arquivo.close();
}

// Menu principal
int main() {
    int opcao;

    while (true) {
        cout << "\n===== GERENCIADOR DE SENHAS =====\n";
        cout << "1 - Adicionar senha\n";
        cout << "2 - Buscar senha\n";
        cout << "3 - Listar servicos\n";
        cout << "4 - Deletar senha\n";
        cout << "0 - Sair\n";
        cout << "Opcao: ";

        cin >> opcao;

        if (opcao == 0) break;

        switch (opcao) {
            case 1: adicionar(); break;
            case 2: buscar(); break;
            case 3: listar(); break;
            case 4: deletar(); break;
            default: cout << "Opcao invalida.\n";
        }
    }

    return 0;
}