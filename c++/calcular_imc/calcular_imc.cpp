#include <iostream>
using namespace std;

int main() {
    float altura, peso, imc;
    
    cout << "digite o seu peso(kg): ";
    cin >> peso;
    cout << "digite a sua altura(cm): ";
    cin >> altura;
    
    imc = peso / (altura * altura);
    
    cout << "\nseu IMC: " << endl;
    
    if (imc < 18.5) {
        cout << "Classificacao: Abaixo do peso" << endl;
    } 
    else if (imc >= 18.5 && imc < 25) {
        cout << "Classificacao: Peso ideal (Parabens!)" << endl;
    } 
    else if (imc >= 25 && imc < 30) {
        cout << "Classificacao: Sobrepeso" << endl;
    } 
    else {
        cout << "Classificacao: Obesidade" << endl;
    }
    
    return 0;
}