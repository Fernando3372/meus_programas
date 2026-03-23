#include <iostream>
using namespace std;

int main() {
float a;
float b;
float resultado;
int op;


    do {
        cout << "\n=== MENU ===\n";
        cout << "1 - Somar\n";
        cout << "2 - Subtrair\n";
        cout << "3 - Multiplicar\n";
        cout << "4 - Dividir\n";
        cout << "0 - Sair\n";
        cout << "Escolha uma opcao: ";
        cin >> op;

        if (op >= 1 && op <= 4) {
            cout << "Digite o primeiro numero: ";
            cin >> a;
            cout << "Digite o segundo numero: ";
            cin >> b;
        }

        switch (op) {
            case 1:
                resultado = a + b;
                cout << "Resultado: " << resultado << endl;
                break;
            case 2:
                resultado = a - b;
                cout << "Resultado: " << resultado << endl;
                break;
            case 3:
                resultado = a * b;
                cout << "Resultado: " << resultado << endl;
                break;
            case 4:
                if (b != 0) {
                    resultado = a / b;
                    cout << "Resultado: " << resultado << endl;
                } else {
                    cout << "Erro: divisao por zero!" << endl;
                }
                break;
            case 0:
                cout << "Saindo..." << endl;
                break;
            default:
                cout << "Opcao invalida!" << endl;
        }

    } while (op != 0);

   return 0;
}