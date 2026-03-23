#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Estrutura de Produto
struct Produto {
    int id;
    string nome;
    double preco;
    int quantidade;
};

// Estrutura de Cliente
struct Cliente {
    int id;
    string nome;
};

// Estrutura de Pedido
struct Pedido {
    int id;
    int clienteId;
    int produtoId;
    int quantidade;
};

// Vetores globais
vector<Produto> estoque;
vector<Cliente> clientes;
vector<Pedido> pedidos;

int proximoIdProduto = 1;
int proximoIdCliente = 1;
int proximoIdPedido = 1;

// Funções de Produto
void adicionarProduto(const string& nome, double preco, int quantidade) {
    Produto p = {proximoIdProduto++, nome, preco, quantidade};
    estoque.push_back(p);
    cout << "Produto " << nome << " cadastrado! ID: " << p.id << "\n";
}

void listarProdutos() {
    cout << "\n--- Produtos no Estoque ---\n";
    for (auto &p : estoque) {
        cout << "ID: " << p.id 
             << " | Nome: " << p.nome 
             << " | Preço: R$ " << p.preco 
             << " | Quantidade: " << p.quantidade << "\n";
    }
    cout << "---------------------------\n";
}

// Funções de Cliente
void adicionarCliente(const string& nome) {
    Cliente c = {proximoIdCliente++, nome};
    clientes.push_back(c);
    cout << "Cliente " << nome << " cadastrado! ID: " << c.id << "\n";
}

void listarClientes() {
    cout << "\n--- Clientes ---\n";
    for (auto &c : clientes) {
        cout << "ID: " << c.id << " | Nome: " << c.nome << "\n";
    }
    cout << "----------------\n";
}

// Funções de Pedido
void criarPedido(int clienteId, int produtoId, int quantidade) {
    // Verificar se produto existe e tem estoque
    for (auto &p : estoque) {
        if (p.id == produtoId) {
            if (p.quantidade >= quantidade) {
                p.quantidade -= quantidade;
                Pedido ped = {proximoIdPedido++, clienteId, produtoId, quantidade};
                pedidos.push_back(ped);
                cout << "Pedido criado! ID: " << ped.id << "\n";
                return;
            } else {
                cout << "Estoque insuficiente!\n";
                return;
            }
        }
    }
    cout << "Produto não encontrado!\n";
}

void listarPedidos() {
    cout << "\n--- Pedidos ---\n";
    for (auto &ped : pedidos) {
        cout << "ID Pedido: " << ped.id 
             << " | Cliente ID: " << ped.clienteId 
             << " | Produto ID: " << ped.produtoId 
             << " | Quantidade: " << ped.quantidade << "\n";
    }
    cout << "----------------\n";
}

// Relatórios automáticos
void relatorio() {
    cout << "\n=== RELATÓRIO AUTOMÁTICO ===\n";
    cout << "Total de produtos: " << estoque.size() << "\n";
    cout << "Total de clientes: " << clientes.size() << "\n";
    cout << "Total de pedidos: " << pedidos.size() << "\n";
    cout << "============================\n";
}

int main() {
    int opcao;
    do {
        cout << "\n--- MENU ESTOQUE ---\n";
        cout << "1. Adicionar produto\n";
        cout << "2. Listar produtos\n";
        cout << "3. Adicionar cliente\n";
        cout << "4. Listar clientes\n";
        cout << "5. Criar pedido\n";
        cout << "6. Listar pedidos\n";
        cout << "7. Relatório automático\n";
        cout << "0. Sair\n";
        cout << "Escolha: ";
        cin >> opcao;

        if (opcao == 1) {
            string nome; double preco; int qtd;
            cout << "Nome do produto: "; cin.ignore(); getline(cin, nome);
            cout << "Preço: "; cin >> preco;
            cout << "Quantidade: "; cin >> qtd;
            adicionarProduto(nome, preco, qtd);
        } else if (opcao == 2) {
            listarProdutos();
        } else if (opcao == 3) {
            string nome;
            cout << "Nome do cliente: "; cin.ignore(); getline(cin, nome);
            adicionarCliente(nome);
        } else if (opcao == 4) {
            listarClientes();
        } else if (opcao == 5) {
            int clienteId, produtoId, qtd;
            cout << "ID do cliente: "; cin >> clienteId;
            cout << "ID do produto: "; cin >> produtoId;
            cout << "Quantidade: "; cin >> qtd;
            criarPedido(clienteId, produtoId, qtd);
        } else if (opcao == 6) {
            listarPedidos();
        } else if (opcao == 7) {
            relatorio();
        }
    } while (opcao != 0);

    cout << "Encerrando sistema...\n";
    return 0;
}
