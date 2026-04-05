#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>

using namespace std;
namespace fs = filesystem;

// Estrutura do Mini Git
const string PASTA_GIT = "meugit";
const string PASTA_COMMITS = "meugit/commits";
const string ARQUIVO_INDEX = "meugit/index.txt";
const string ARQUIVO_LOG = "meugit/log.txt";

// ================= INIT =================
void init() {
    fs::create_directory(PASTA_GIT);
    fs::create_directory(PASTA_COMMITS);

    ofstream index(ARQUIVO_INDEX);
    ofstream log(ARQUIVO_LOG);

    cout << "Repositorio inicializado.\n";
}

// ================= ADD =================
bool arquivoJaAdicionado(const string& nomeArquivo) {
    ifstream index(ARQUIVO_INDEX);
    string linha;

    while (getline(index, linha)) {
        if (linha == nomeArquivo)
            return true;
    }
    return false;
}

void add(const string& nomeArquivo) {
    if (!fs::exists(nomeArquivo)) {
        cout << "Arquivo nao existe.\n";
        return;
    }

    if (arquivoJaAdicionado(nomeArquivo)) {
        cout << "Arquivo ja adicionado.\n";
        return;
    }

    ofstream index(ARQUIVO_INDEX, ios::app);
    index << nomeArquivo << endl;
    index.close();

    cout << "Arquivo adicionado: " << nomeArquivo << endl;
}

// ================= COMMIT =================
int contarCommits() {
    int total = 0;
    if (fs::exists(PASTA_COMMITS)) {
        for (auto& p : fs::directory_iterator(PASTA_COMMITS))
            total++;
    }
    return total;
}

void commit(const string& mensagem) {
    ifstream index(ARQUIVO_INDEX);
    if (!index) {
        cout << "Nada para commitar.\n";
        return;
    }

    int numeroCommit = contarCommits() + 1;
    string pastaCommit = PASTA_COMMITS + "/" + to_string(numeroCommit);
    fs::create_directory(pastaCommit);

    string arquivo;
    while (getline(index, arquivo)) {
        if (!arquivo.empty()) {
            fs::copy_file(
                arquivo,
                pastaCommit + "/" + arquivo,
                fs::copy_options::overwrite_existing
            );
        }
    }
    index.close();

    // Limpa o index
    ofstream limpar(ARQUIVO_INDEX);
    limpar.close();

    // Registra no log
    ofstream log(ARQUIVO_LOG, ios::app);
    log << numeroCommit << " - " << mensagem << endl;
    log.close();

    cout << "Commit criado: " << numeroCommit << endl;
}

// ================= LOG =================
void mostrarLog() {
    ifstream log(ARQUIVO_LOG);
    string linha;

    while (getline(log, linha)) {
        cout << linha << endl;
    }
}

// ================= CHECKOUT =================
void checkout(int numeroCommit) {
    string pastaCommit = PASTA_COMMITS + "/" + to_string(numeroCommit);

    if (!fs::exists(pastaCommit)) {
        cout << "Commit nao existe.\n";
        return;
    }

    for (auto& p : fs::directory_iterator(pastaCommit)) {
        fs::copy_file(
            p.path(),
            p.path().filename(),
            fs::copy_options::overwrite_existing
        );
    }

    cout << "Projeto restaurado para o commit " << numeroCommit << endl;
}

// ================= MAIN =================
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Comandos: init | add | commit | log | checkout\n";
        return 0;
    }

    string comando = argv[1];

    if (comando == "init") {
        init();
    }
    else if (comando == "add" && argc == 3) {
        add(argv[2]);
    }
    else if (comando == "commit" && argc == 3) {
        commit(argv[2]);
    }
    else if (comando == "log") {
        mostrarLog();
    }
    else if (comando == "checkout" && argc == 3) {
        checkout(stoi(argv[2]));
    }
    else {
        cout << "Comando invalido.\n";
    }

    return 0;
}