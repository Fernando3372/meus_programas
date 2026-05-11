#include <iostream>
#include <fstream>
#include <string>

using namespace std;

namespace EditorTexto {
    const string ARQUIVO_PADRAO = "texto.txt";

    void escrever(const string& nomeArquivo) {
        ofstream arquivo(nomeArquivo, ios::app);
        if (!arquivo.is_open()) {
            cerr << "Erro: nao foi possivel abrir o arquivo." << endl;
            return;
        }

        cout << "Digite o texto (digite 'SAIR' para terminar):" << endl;
        string linha;
        while (true) {
            getline(cin, linha);
            if (linha == "SAIR") break;
            arquivo << linha << endl;
        }
    }

    void ler(const string& nomeArquivo) {
        ifstream arquivo(nomeArquivo);
        if (!arquivo.is_open()) {
            cerr << "Erro: arquivo nao encontrado." << endl;
            return;
        }

        int totalLinhas = 0;
        int totalCaracteres = 0;
        string linha;

        while (getline(arquivo, linha)) {
            cout << linha << endl;
            totalLinhas++;
            totalCaracteres += static_cast<int>(linha.size());
        }

        cout << "\nResumo do arquivo:" << endl;
        cout << "Linhas: " << totalLinhas << endl;
        cout << "Caracteres: " << totalCaracteres << endl;
    }

    void buscarPalavra(const string& nomeArquivo, const string& palavra) {
        ifstream arquivo(nomeArquivo);
        if (!arquivo.is_open()) {
            cerr << "Erro: arquivo nao encontrado." << endl;
            return;
        }

        string linha;
        int numeroLinha = 0;
        bool encontrada = false;

        while (getline(arquivo, linha)) {
            numeroLinha++;
            if (linha.find(palavra) != string::npos) {
                cout << "Encontrada na linha " << numeroLinha << ": " << linha << endl;
                encontrada = true;
            }
        }

        if (!encontrada) {
            cout << "Palavra '" << palavra << "' nao encontrada." << endl;
        }
    }
}

void mostrarMenu() {
    cout << "\n--- Editor de Texto ---" << endl;
    cout << "1. Escrever no arquivo" << endl;
    cout << "2. Ler arquivo" << endl;
    cout << "3. Buscar palavra" << endl;
    cout << "4. Sair" << endl;
    cout << "Escolha uma opcao: ";
}

int main() {
    int opcao;
    string palavra;

    do {
        mostrarMenu();
        cin >> opcao;
        cin.ignore(); // limpar buffer

        switch (opcao) {
            case 1:
                EditorTexto::escrever(EditorTexto::ARQUIVO_PADRAO);
                break;
            case 2:
                EditorTexto::ler(EditorTexto::ARQUIVO_PADRAO);
                break;
            case 3:
                cout << "Digite a palavra para buscar: ";
                getline(cin, palavra);
                EditorTexto::buscarPalavra(EditorTexto::ARQUIVO_PADRAO, palavra);
                break;
            case 4:
                cout << "Encerrando programa..." << endl;
                break;
            default:
                cout << "Opcao invalida. Tente novamente." << endl;
        }
    } while (opcao != 4);

    return 0;
}