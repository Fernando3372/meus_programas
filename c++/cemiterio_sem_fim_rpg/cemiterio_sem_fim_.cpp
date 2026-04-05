#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// ===== ESTRUTURAS =====
struct Jogador {
    int vida, vidaMax;
    int ataque;
    int xp;
    int nivel;
    int gold;
    int pocoes, bifes, paes, vinhos;
    int monstrosMortos;
    string espada;
};

struct Monstro {
    string nome;
    int vida, vidaMax;
    int ataque;
    int xp;
    int gold;
};

// ===== FUNÇÕES =====
void pausa() {
    cout << "\nPressione ENTER para continuar...";
    cin.ignore(10000, '\n');
    cin.get();
}

// ===== STATUS =====
void status(Jogador j) {
    system("clear");
    cout << "======= STATUS =======\n";
    cout << "Nivel: " << j.nivel << endl;
    cout << "Vida: " << j.vida << "/" << j.vidaMax << endl;
    cout << "Ataque: " << j.ataque << endl;
    cout << "XP: " << j.xp << "/100\n";
    cout << "Gold: " << j.gold << endl;
    cout << "Monstros derrotados: " << j.monstrosMortos << endl;
    pausa();
}

// ===== INVENTARIO =====
void inventario(Jogador j) {
    system("clear");
    cout << "===== INVENTARIO =====\n";
    cout << "\nEspada equipada: " << j.espada << "\n\n";
    cout << "Pocao: " << j.pocoes << endl;
    cout << "Bife:  " << j.bifes << endl;
    cout << "Pao:   " << j.paes << endl;
    cout << "Vinho: " << j.vinhos << endl;
    pausa();
}

// ===== USAR ITEM =====
void usarItem(Jogador &j) {
    system("clear");

    cout << "========= ITENS =========\n";
    cout << "1 - Pocao  x" << j.pocoes << " (cura total)\n";
    cout << "2 - Bife   x" << j.bifes  << " (+100 vida)\n";
    cout << "3 - Pao    x" << j.paes   << " (+50 vida)\n";
    cout << "4 - Vinho  x" << j.vinhos << " (+25 vida)\n";
    cout << "0 - Voltar\n";
    cout << "Opcao: ";

    int op;
    cin >> op;
    
   if (cin.fail()) {
    cin.clear();
    cin.ignore(1000, '\n');
    cout << "\nOpcao invalida!\n";
    pausa();
    }

    if (op == 1 && j.pocoes > 0) {
        j.vida = j.vidaMax;
        j.pocoes--;
        cout << "\nVida totalmente restaurada!\n";
    }
    else if (op == 2 && j.bifes > 0) {
        j.vida += 100;
        j.bifes--;
    }
    else if (op == 3 && j.paes > 0) {
        j.vida += 50;
        j.paes--;
    }
    else if (op == 4 && j.vinhos > 0) {
        j.vida += 25;
        j.vinhos--;
    }
    else if (op != 0) {
        cout << "\nItem invalido!\n";
    }

    if (j.vida > j.vidaMax)
        j.vida = j.vidaMax;

    pausa();
}

// ===== LOJA =====
void loja(Jogador &j) {
    system("clear");

    cout << "=========== LOJA ===========\n";
    cout << "Gold: " << j.gold << "\n\n";

    cout << "------ ESPADAS ------\n";
    cout << "1 - Espada de Madeira  (30 gold   | 10 dano)\n";
    cout << "2 - Espada de Ferro    (80 gold   | 20 dano)\n";
    cout << "3 - Espada de Chamas   (200 gold  | 35 dano)\n";
    cout << "4 - Espada do Infinito (1000 gold | 100 dano)\n\n";

    cout << "------ ITENS ------\n";
    cout << "5 - Pocao (100 gold | VIDAMAX)\n";
    cout << "6 - Bife  (50 gold  | +100 vida)\n";
    cout << "7 - Pao   (25 gold  | +50 vida)\n";
    cout << "8 - Vinho (10 gold  | +25 vida)\n\n";

    cout << "0 - Voltar\n";
    cout << "Opcao: ";

    int op;
    cin >> op;
    
   if (cin.fail()) {
    cin.clear();
    cin.ignore(1000, '\n');
    cout << "\nOpcao invalida!\n";
    pausa();
    }

    if (op == 1 && j.gold >= 30) {
        j.gold -= 30;
        j.ataque = 10;
        j.espada = "Espada de Madeira";
    }
    else if (op == 2 && j.gold >= 80) {
        j.gold -= 80;
        j.ataque = 20;
        j.espada = "Espada de Ferro";
    }
    else if (op == 3 && j.gold >= 200) {
        j.gold -= 200;
        j.ataque = 35;
        j.espada = "Espada de Chamas";
    }
    else if (op == 4 && j.gold >= 1000) {
        j.gold -= 1000;
        j.ataque = 100;
        j.espada = "Espada do Infinito";
    }
    else if (op == 5 && j.gold >= 100) {
        j.gold -= 100;
        j.pocoes++;
    }
    else if (op == 6 && j.gold >= 50) {
        j.gold -= 50;
        j.bifes++;
    }
    else if (op == 7 && j.gold >= 25) {
        j.gold -= 25;
        j.paes++;
    }
    else if (op == 8 && j.gold >= 10) {
        j.gold -= 10;
        j.vinhos++;
    }

    pausa();
}

// ===== GERAR MONSTRO =====
Monstro gerarMonstro(int nivel) {
    int r = rand() % 100;

    if (r < 1)
        return {"Dragao🐲", 200, 200, 45, 100, 200};

    else if (r < 7)   // 1–6
        return {"Golem🗿", 150, 150, 30, 60, 80};

    else if (r < 15)  // 7–14
        return {"Mago🧙", 75, 75, 20, 45, 50};

    else if (r < 35)  // 15–34
        return {"Zumbi🧟", 55, 55, 12, 25, 25};

    else if (r < 65)  // 35–64
        return {"Esqueleto💀", 35, 35, 8, 18, 18};

    else              // 65–99
        return {"Aranha🕷️", 20, 20, 5, 10, 10};
}

// ===== COMBATE =====
void lutar(Jogador &j) {
    Monstro m = gerarMonstro(j.nivel);

    system("clear");
    cout << "⚔️ Um " << m.nome << " apareceu!\n";
    pausa();

    while (j.vida > 0 && m.vida > 0) {
        system("clear");
        cout << "======= COMBATE =======\n";
        cout << "Sua Vida: " << j.vida << "/" << j.vidaMax << endl;
        cout << "Vida do " << m.nome << ": " << m.vida << "/" << m.vidaMax << "\n\n";
        cout << "1 - Atacar\n2 - Usar Item\n3 - Fugir\nOpcao: ";

        int op;
        cin >> op;
        
       if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nOpcao invalida!\n";
        pausa();
        }

        if (op == 1) {
            int dano = j.ataque;
            int variacao = rand() % 41 - 20;
            dano += dano * variacao / 100;

            bool critico = rand() % 100 < 10;
            if (critico) dano *= 2;

            m.vida -= dano;
            cout << "\nVoce causou " << dano << " de dano";
            if (critico) cout << " 💥 CRITICO!";
            cout << endl;

            if (m.vida > 0) {
                j.vida -= m.ataque;
                cout << m.nome << " causou " << m.ataque << " de dano.\n";
            }
            pausa();
        }
        else if (op == 2) usarItem(j);
        else if (op == 3) {
            if (rand() % 100 < 50) {
                cout << "\n🏃 Voce conseguiu fugir!\n";
                pausa();
                return;
            } else {
                j.vida -= m.ataque;
                cout << "\n❌ Fuga falhou! Voce sofreu " << m.ataque << " de dano.\n";
                pausa();
            }
        }
        else {
            cout << "\nOpcao invalida!\n";
            pausa();
        }
    }

    if (j.vida > 0) {
    system("clear");
        cout << "\n🏆 Voce derrotou o " << m.nome << "!\n";
        cout << "Ganhou " << m.xp << " XP e " << m.gold << " Gold.\n";
        
        j.xp += m.xp;
        j.gold += m.gold;
        j.monstrosMortos++;

        if (j.xp >= 100) {
            j.xp -= 100;
            j.nivel++;
            j.vidaMax += 10;
            j.ataque += 1;
            j.vida = j.vidaMax;

            cout << "✨ Subiu para o nivel " << j.nivel << "!\n";
            cout << "+10 Vida Max | +1 Ataque\n";
        }
        pausa();
    }

    if (j.vida <= 0) {
        cout << "\nITS OVER ☠️\n";
        pausa();
        exit(0);
    }
}

// ===== MAIN =====
int main() {
    srand(time(0));

    cout << "=====================================\n";
    cout << "==== O CEMITERIO SEM FIM - RPG ======\n";
    cout << "=====================================\n\n";
    cout << "Ha muito tempo, este cemiterio foi amaldiçoado.\n";
    cout << "Os mortos nunca descansam.\n";
    cout << "Criaturas surgem todas as noites.\n\n";
    cout << "Seu objetivo nao e salvar o mundo.\n";
    cout << "E apenas sobreviver.\n\n";
    cout << "Pressione ENTER para iniciar...";
    cin.get();

    Jogador j = {100, 100, 5, 0, 1, 25, 1, 0, 0, 0, 0, "Sem espada"};

    while (true) {
        system("clear");
        cout << "1 - batalhar\n2 - Status\n3 - Inventario\n4 - Usar Item\n5 - Loja\n0 - Sair\nOpcao: ";
        int op;
        cin >> op;
        
        if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nOpcao invalida!\n";
        pausa();
        continue;
        }

        if (op == 0) break;
        if (op == 1) lutar(j);
        if (op == 2) status(j);
        if (op == 3) inventario(j);
        if (op == 4) usarItem(j);
        if (op == 5) loja(j);
    }

    return 0;
}