#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

// Estrutura do registro salvo no arquivo
struct Registro {
    int id;
    char nome[50];
    int idade;
    bool deletado;
};

const char* NOME_ARQUIVO = "banco_dados.bin";

// Inserir um novo registro
void inserirRegistro(int id, const char* nome, int idade) {
    fstream arquivo(NOME_ARQUIVO, ios::out | ios::app | ios::binary);

    Registro r;
    r.id = id;
    strncpy(r.nome, nome, sizeof(r.nome));
    r.idade = idade;
    r.deletado = false;

    arquivo.write((char*)&r, sizeof(Registro));
    arquivo.close();

    cout << "Registro inserido com sucesso.\n";
}

// Buscar registro pelo ID
void buscarRegistro(int id) {
    ifstream arquivo(NOME_ARQUIVO, ios::binary);
    Registro r;

    while (arquivo.read((char*)&r, sizeof(Registro))) {
        if (r.id == id && !r.deletado) {
            cout << "ID: " << r.id
                 << " | Nome: " << r.nome
                 << " | Idade: " << r.idade << endl;
            arquivo.close();
            return;
        }
    }

    cout << "Registro não encontrado.\n";
    arquivo.close();
}

// Listar todos os registros
void listarRegistros() {
    ifstream arquivo(NOME_ARQUIVO, ios::binary);
    Registro r;

    while (arquivo.read((char*)&r, sizeof(Registro))) {
        if (!r.deletado) {
            cout << "ID: " << r.id
                 << " | Nome: " << r.nome
                 << " | Idade: " << r.idade << endl;
        }
    }

    arquivo.close();
}

// Deletar registro (marcação lógica)
void deletarRegistro(int id) {
    fstream arquivo(NOME_ARQUIVO, ios::in | ios::out | ios::binary);
    Registro r;

    while (arquivo.read((char*)&r, sizeof(Registro))) {
        if (r.id == id && !r.deletado) {
            r.deletado = true;

            // Volta o ponteiro para reescrever o registro
            arquivo.seekp(-sizeof(Registro), ios::cur);
            arquivo.write((char*)&r, sizeof(Registro));

            cout << "Registro deletado.\n";
            arquivo.close();
            return;
        }
    }

    cout << "Registro não encontrado.\n";
    arquivo.close();
}

int main() {
    int opcao;

    while (true) {
        cout << "\n===== MENU =====\n";
        cout << "1 - Inserir dados\n";
        cout << "2 - Buscar por ID\n";
        cout << "3 - Listar todos\n";
        cout << "4 - Deletar\n";
        cout << "0 - Sair\n";
        cout << "Escolha: ";

        cin >> opcao;

        if (opcao == 0) break;

        if (opcao == 1) {
            int id, idade;
            char nome[50];
            cout << "ID Nome Idade: ";
            cin >> id >> nome >> idade;
            inserirRegistro(id, nome, idade);
        }
        else if (opcao == 2) {
            int id;
            cout << "ID: ";
            cin >> id;
            buscarRegistro(id);
        }
        else if (opcao == 3) {
            listarRegistros();
        }
        else if (opcao == 4) {
            int id;
            cout << "ID: ";
            cin >> id;
            deletarRegistro(id);
        }
    }

    return 0;
}