#include <iostream>
using namespace std;

class banco {
private:
float reais;
public:
banco() {
reais = 0;
}
void sacar(float valor) {
if (valor > 0 && valor <= reais) {
    reais -= valor;
    cout << "Saque de " << valor << " realizado com sucesso!" << endl;
} else {
    cout << "valor insuficiente" << endl;
}
}
void depositar(float valor) {
if (valor > 0) {
    reais += valor;
    cout << "deposito de " << valor << "realizado com sucesso" << endl;
} else {
    cout << "valor para deposito invalido." << endl;
}
}
void consulta() {
    cout << "Saldo atual: R$ " << reais << endl;
}
};

int main() {
    banco conta;            // cria objeto da classe Banco
    conta.consulta();       // mostra saldo inicial
    conta.depositar(100);   // deposita 100
    conta.consulta();       // mostra saldo
    conta.sacar(40);        // saca 40
    conta.consulta();       // mostra saldo final

    return 0;
}
