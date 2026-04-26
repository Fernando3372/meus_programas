// ============================================================
//  CEMITERIO SEM FIM - RPG
//  Compilar: g++ -std=c++17 -o rpg rpg.cpp
// ============================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>

using namespace std;

// ============================================================
//  ENUMS
// ============================================================

enum class ClassePersonagem { GUERREIRO, ASSASSINO, MAGO, LADINO, ARQUEIRO };
enum class TipoDano         { FISICO, FOGO, GELO, SOMBRA, INFINITO };
enum class TipoMonstro      { NORMAL, MAGO_INIMIGO, GUERREIRO_INIMIGO, ELITE };

// ============================================================
//  ESTRUTURA DE ARMA
// ============================================================

struct Arma {
    string   nome;
    int      danoBase;
    TipoDano tipo;
    float    multiplicadorCritico;
    float    bonusPrecisao;   // besta e arqueiro
    float    bonusCritico;    // machado, adaga
    string   descricao;
};

// ============================================================
//  ESTRUTURAS PRINCIPAIS
// ============================================================

struct Jogador {
    int  vida, vidaMax;
    int  ataque;
    int  xp, xpProximoNivel;
    int  nivel;
    int  gold;
    int  pocoes, bifes, paes, vinhos;
    int  monstrosMortos;
    Arma armaEquipada;

    ClassePersonagem classe;
    string nomeClasse;

    float precisao;
    float chanceCritico;
    float multiplicadorCritico;

    float bonusGold;

    // dano continuo de fogo
    int turnosFogo;
    int danoPorTurnoFogo;
    // congelado
    int turnosCongelado;
    // roubo de vida (sombra)
    bool temRouboDeVida;
    // infinito: ciclo
    int cicloInfinito; // 0=fogo,1=gelo,2=sombra
};

struct Monstro {
    string     nome;
    int        vida, vidaMax;
    int        ataque;
    int        xp;
    int        gold;
    TipoMonstro tipo;
    int        mana;
    bool       ehBoss;
    // dano continuo no jogador
    int turnosFogoInimigo;
    int danoPorTurnoFogoInimigo;
    int turnosGeloInimigo;
};

// ============================================================
//  UTILITARIOS
// ============================================================

void pausa() {
    cout << "\nPressione ENTER para continuar...";
    cin.ignore(10000, '\n');
    cin.get();
}

// Barra de vida visual com blocos unicode
string barraVida(int atual, int maximo, int tam = 20) {
    int cheio = (int)round((float)atual / maximo * tam);
    cheio = max(0, min(tam, cheio));
    string b = "[";
    for (int i = 0; i < tam; i++) 
        b += (i < cheio) ? "#" : ".";
    b += "]";
    return b;
}

string nomeTipo(TipoDano t) {
    switch (t) {
        case TipoDano::FISICO:   return "Fisico";
        case TipoDano::FOGO:     return "Fogo";
        case TipoDano::GELO:     return "Gelo";
        case TipoDano::SOMBRA:   return "Sombra";
        case TipoDano::INFINITO: return "Infinito";
        default:                 return "?";
    }
}

// ============================================================
//  ARMAS
// ============================================================

Arma getArma(const string& nome) {
    //  nome            danoBase  tipo              multCrit  bonusPrec  bonusCrit  desc
    if (nome == "Espada de Madeira")
        return {"Espada de Madeira",  10, TipoDano::FISICO,   1.5f, 0.00f, 0.00f, "Uma espada simples."};
    if (nome == "Espada de Ferro")
        return {"Espada de Ferro",    20, TipoDano::FISICO,   1.8f, 0.00f, 0.00f, "Forjada em ferro puro."};
    if (nome == "Machado")
        return {"Machado",            30, TipoDano::FISICO,   2.5f, 0.00f, 0.10f, "Alto critico. Devastador."};
    if (nome == "Adaga")
        return {"Adaga",              22, TipoDano::FISICO,   2.8f, 0.00f, 0.12f, "Critico elevado. Rapida."};
    if (nome == "Arco")
        return {"Arco",               24, TipoDano::FISICO,   2.0f, 0.00f, 0.00f, "Mais dano a distancia."};
    if (nome == "Besta")
        return {"Besta",              25, TipoDano::FISICO,   2.0f, 0.20f, 0.00f, "Aumenta precisao em 20%."};
    if (nome == "Cajado de Fogo")
        return {"Cajado de Fogo",     28, TipoDano::FOGO,     2.0f, 0.00f, 0.00f, "Chamas que queimam por turnos."};
    if (nome == "Cajado de Gelo")
        return {"Cajado de Gelo",     25, TipoDano::GELO,     1.9f, 0.00f, 0.00f, "Pode congelar inimigos."};
    if (nome == "Cajado das Sombras")
        return {"Cajado das Sombras", 35, TipoDano::SOMBRA,   2.2f, 0.00f, 0.00f, "Rouba vida do inimigo."};
    if (nome == "Espada do Infinito")
        return {"Espada do Infinito", 70, TipoDano::INFINITO, 3.0f, 0.00f, 0.00f, "Poder absoluto. Alterna fogo/gelo/sombra."};
    if (nome == "Punho")
        return {"Punho", 5, TipoDano::FISICO, 1.5f, 0.00f, 0.05f, "Seus proprios punhos."};
    // fallback
    return {"Punho", 5, TipoDano::FISICO, 1.5f, 0.00f, 0.05f, "Seus proprios punhos."};

}

// ============================================================
//  ESCOLHA DE CLASSE
// ============================================================

void escolherClasse(Jogador& j) {
    system("cls");
    cout << "==============================\n";
    cout << "     ESCOLHA SUA CLASSE\n";
    cout << "==============================\n\n";
    cout << "1 - Guerreiro\n";
    cout << "   Tanque. Vida maxima muito alta. comeca com Espada\n";
    cout << "   Critico: 12% | Precisao: 78%\n\n";
    cout << "2 - Assassino\n";
    cout << "   Fragil, mas critico devastador.\n";
    cout << "   Critico: 40% x3.0 | Precisao: 85%\n\n";
    cout << "3 - Mago\n";
    cout << "   Vida baixa, precisao ruim e Critico alto. comeca com Cajado.\n";
    cout << "   Critico: 30% | Precisao: 68%\n\n";
    cout << "4 - Ladino\n";
    cout << "   Equilibrado. Ganha 50%% mais gold.\n";
    cout << "   Critico: 28% | Precisao: 83%\n\n";
    cout << "5 - Arqueiro\n";
    cout << "   Raramente erra. Critico baixo. Comeca com Arco.\n";
    cout << "   Critico: 18% | Precisao: 97%\n\n";
    cout << "Opcao: ";

    int op; cin >> op;
    if (cin.fail()) { cin.clear(); cin.ignore(1000,'\n'); op = 1; }

    // defaults zerados
    j.turnosFogo = 0; j.danoPorTurnoFogo = 0;
    j.turnosCongelado = 0; j.temRouboDeVida = false;
    j.cicloInfinito = 0; 

    switch (op) {
        case 2:
            j.classe = ClassePersonagem::ASSASSINO;
            j.nomeClasse = "Assassino";
            j.vidaMax = 80; j.vida = 80; j.ataque = 13;
            j.precisao = 0.85f; j.chanceCritico = 0.40f;
            j.multiplicadorCritico = 3.0f;
            j.armaEquipada = getArma("Punho");
            j.bonusGold = 1.0f;
            break;
        case 3:
            j.classe = ClassePersonagem::MAGO;
            j.nomeClasse = "Mago";
            j.vidaMax = 75; j.vida = 75; j.ataque = 14;
            j.precisao = 0.68f; j.chanceCritico = 0.30f;
            j.multiplicadorCritico = 3.0f;
            j.armaEquipada = getArma("Cajado de Fogo");
            j.bonusGold = 1.0f;
            break;
        case 4:
            j.classe = ClassePersonagem::LADINO;
            j.nomeClasse = "Ladino";
            j.vidaMax = 90; j.vida = 90; j.ataque = 10;
            j.precisao = 0.83f; j.chanceCritico = 0.28f;
            j.multiplicadorCritico = 2.5f;
            j.armaEquipada = getArma("Punho");
            j.bonusGold = 1.5f;
            break;
        case 5:
            j.classe = ClassePersonagem::ARQUEIRO;
            j.nomeClasse = "Arqueiro";
            j.vidaMax = 90; j.vida = 90; j.ataque = 11;
            j.precisao = 0.97f; j.chanceCritico = 0.18f;
            j.multiplicadorCritico = 2.0f;
            j.armaEquipada = getArma("Arco");
            j.bonusGold = 1.0f;
            break;
        default: // 1 - Guerreiro
            j.classe = ClassePersonagem::GUERREIRO;
            j.nomeClasse = "Guerreiro";
            j.vidaMax = 150; j.vida = 150; j.ataque = 15;
            j.precisao = 0.78f; j.chanceCritico = 0.12f;
            j.multiplicadorCritico = 1.8f;
            j.armaEquipada = getArma("Espada de Madeira");
            j.bonusGold = 1.0f;
            break;
    }
}

// ============================================================
//  STATUS (completo)
// ============================================================

void status(Jogador j) {
    system("cls");
    cout << "============= STATUS =============\n";
    cout << "Classe  : " << j.nomeClasse << "\n";
    cout << "Nivel   : " << j.nivel << "\n";
    cout << "XP      : " << j.xp << "/" << j.xpProximoNivel << "\n";
    cout << "Vida    : " << barraVida(j.vida, j.vidaMax)
         << " " << j.vida << "/" << j.vidaMax << "\n";
    cout << "Ataque  : " << j.ataque << "\n";
    cout << "Precisao: " << (int)(j.precisao * 100) << "%\n";
    cout << "Critico : " << (int)(j.chanceCritico * 100)
         << "% x" << j.multiplicadorCritico << "\n";
    cout << "Gold    : " << j.gold << "\n";
    cout << "Mortos  : " << j.monstrosMortos << "\n";
    if (j.classe == ClassePersonagem::LADINO)
        cout << "Bonus Gold: +" << (int)((j.bonusGold - 1.0f) * 100) << "%\n";
    cout << "\n--- Arma Equipada ---\n";
    cout << "Nome    : " << j.armaEquipada.nome << "\n";
    cout << "Tipo    : " << nomeTipo(j.armaEquipada.tipo) << "\n";
    cout << "Dano    : " << j.armaEquipada.danoBase << "\n";
    cout << "Desc    : " << j.armaEquipada.descricao << "\n";
    pausa();
}

// ============================================================
//  INVENTARIO
// ============================================================

void inventario(Jogador j) {
    system("cls");
    cout << "========= INVENTARIO =========\n\n";
    cout << "Arma     : " << j.armaEquipada.nome << "\n";
    cout << "Tipo dano: " << nomeTipo(j.armaEquipada.tipo) << "\n";
    cout << "Dano base: " << j.armaEquipada.danoBase << "\n";
    cout << "Desc     : " << j.armaEquipada.descricao << "\n\n";
    cout << "--- Consumiveis ---\n";
    cout << "Pocao x" << j.pocoes << " (restaura vida total)\n";
    cout << "Bife  x" << j.bifes  << " (+100 vida)\n";
    cout << "Pao   x" << j.paes   << " (+50 vida)\n";
    cout << "Vinho x" << j.vinhos << " (+25 vida)\n";
    pausa();
}

// ============================================================
//  USAR ITEM
// ============================================================

void usarItem(Jogador& j) {
    system("cls");
    cout << "========= USAR ITEM =========\n";
    cout << "1 - Pocao  x" << j.pocoes << " (cura total)\n";
    cout << "2 - Bife   x" << j.bifes  << " (+100 vida)\n";
    cout << "3 - Pao    x" << j.paes   << " (+50 vida)\n";
    cout << "4 - Vinho  x" << j.vinhos << " (+25 vida)\n";
    cout << "0 - Voltar\nOpcao: ";

    int op; cin >> op;
    if (cin.fail()) { cin.clear(); cin.ignore(1000,'\n');
        cout << "\nOpcao invalida!\n"; pausa(); return; }

    if      (op == 1 && j.pocoes > 0) { j.vida = j.vidaMax; j.pocoes--; cout << "\nVida restaurada!\n"; }
    else if (op == 2 && j.bifes  > 0) { j.vida += 100; j.bifes--;  cout << "\nBife usado: +100 vida!\n"; }
    else if (op == 3 && j.paes   > 0) { j.vida += 50;  j.paes--;   cout << "\nPao usado: +50 vida!\n";  }
    else if (op == 4 && j.vinhos > 0) { j.vida += 25;  j.vinhos--; cout << "\nVinho usado: +25 vida!\n";}
    else if (op != 0) cout << "\nItem invalido ou sem estoque!\n";

    if (j.vida > j.vidaMax) j.vida = j.vidaMax;
    pausa();
}

// ============================================================
//  LOJA PRINCIPAL (balanceada)
// ============================================================

void loja(Jogador& j) {
    system("cls");
    cout << "============== LOJA ==============\n";
    cout << "Gold: " << j.gold << "\n\n";
    cout << "------ ARMAS ------\n";
    cout << "1 - Espada de Madeira (30 gold  | 10 dano | Fisico)\n";
    cout << "2 - Espada de Ferro   (80 gold  | 20 dano | Fisico)\n";
    cout << "3 - Adaga             (120 gold | 22 dano | Fisico, critico elevado)\n";
    cout << "4 - Arco              (100 gold | 24 dano | Fisico)\n";
    cout << "5 - Cajado de Fogo    (240 gold | 28 dano | Fogo)\n";
    cout << "6 - Cajado de Gelo    (200 gold | 25 dano | Gelo)\n\n";
    cout << "------ ITENS ------\n";
    cout << "7 - Pocao   (200 gold | Restaura vida total)\n";
    cout << "8 - Bife    (80 gold  | +100 vida)\n";
    cout << "9 - Pao     (40 gold  | +50 vida)\n";
    cout << "10- Vinho   (20 gold  | +25 vida)\n\n";
    cout << "0 - Voltar\nOpcao: ";

    int op; cin >> op;
    if (cin.fail()) { cin.clear(); cin.ignore(1000,'\n');
        cout << "\nOpcao invalida!\n"; pausa(); return; }

    struct Compra { int custo; const char* nome; };
    Compra armas[] = {
        {30,"Espada de Madeira"},{80,"Espada de Ferro"},
        {120,"Adaga"},
        {100,"Arco"},
        {240,"Cajado de Fogo"},{200,"Cajado de Gelo"}
    };

    if (op >= 1 && op <= 6) {
        Compra& c = armas[op-1];
        if (j.gold >= c.custo) {
            j.gold -= c.custo;
            j.armaEquipada = getArma(c.nome);
            // aplica bonus de precisao da arma (ex: Besta)
            float novaPrecisao = min(0.99f, j.precisao + j.armaEquipada.bonusPrecisao);
            // aplica bonus de critico da arma
            float novaCrit = min(0.70f, j.chanceCritico + j.armaEquipada.bonusCritico);
            j.precisao = novaPrecisao;
            j.chanceCritico = novaCrit;
            cout << "\nEquipado: " << c.nome << "!\n";
        } else cout << "\nGold insuficiente!\n";
    }
    else if (op == 7  && j.gold >= 200) { j.gold -= 200; j.pocoes++; cout << "\nPocao comprada!\n"; }
    else if (op == 8 && j.gold >= 80)  { j.gold -= 80;  j.bifes++;  cout << "\nBife comprado!\n";  }
    else if (op == 9 && j.gold >= 40)  { j.gold -= 40;  j.paes++;   cout << "\nPao comprado!\n";   }
    else if (op == 10 && j.gold >= 20)  { j.gold -= 20;  j.vinhos++; cout << "\nVinho comprado!\n"; }
    else if (op != 0) cout << "\nGold insuficiente!\n";

    pausa();
}

// ============================================================
//  COMERCIANTE RARO (itens exclusivos)
// ============================================================

void comercianteRaro(Jogador& j) {
    system("cls");
    cout << "Um homem encurvado emerge das sombras...\n";
    int r = rand() % 100; // número aleatório entre 0 e 99
    if (r < 50) {
        cout << "Tenho itens raros... mas não confie em mim.\"\n\n";
    } else if (r < 80) {
        cout << "O que vendo pode salvar sua vida... ou acabar com ela.\"\n\n";
    } else {
        cout << "Nem todos deveriam ver o que guardo...\"\n\n";
    }
    cout << "Gold: " << j.gold << "\n\n";
    cout << "-- Itens Exclusivos --\n";
    cout << "1 - Espada do Infinito  (5000 gold | 70 dano | Infinito, Fogo/Gelo/Sombra)\n";
    cout << "2 - Cajado das Sombras  (1000 gold | 35 dano | Sombra, rouba vida)\n";
    cout << "3 - Machado             (250 gold  | 30 dano | Fisico, critico alto)\n";
    cout << "4 - Besta               (120 gold  | 25 dano | +20%% precisao)\n";
    cout << "5 - Elixir Supremo      (500 gold  | Cura total + VidaMax +20)\n";
    cout << "0 - Dispensar\nOpcao: ";

    int op; cin >> op;
    if (cin.fail()) { cin.clear(); cin.ignore(1000,'\n'); pausa(); return; }

    auto equiparArma = [&](const string& nome) {
        j.armaEquipada = getArma(nome);
        float novaPrecisao = min(0.99f, j.precisao + j.armaEquipada.bonusPrecisao);
        float novaCrit = min(0.70f, j.chanceCritico + j.armaEquipada.bonusCritico);
        j.precisao = novaPrecisao;
        j.chanceCritico = novaCrit;
        cout << "\nEquipado: " << nome << "!\n";
    };

    if      (op == 1 && j.gold >= 5000) { j.gold -= 5000; equiparArma("Espada do Infinito"); }
    else if (op == 2 && j.gold >= 1000)  { j.gold -= 1000;  equiparArma("Cajado das Sombras"); }
    else if (op == 3 && j.gold >= 250)  { j.gold -= 250;  equiparArma("Machado"); }
    else if (op == 4 && j.gold >= 120)  { j.gold -= 120;  equiparArma("Besta"); }
    else if (op == 5 && j.gold >= 500)  { j.gold -= 500;  j.vidaMax += 20; j.vida = j.vidaMax;
                                          cout << "\nVidaMax +20 e vida restaurada!\n"; }
    else if (op != 0) cout << "\nGold insuficiente!\n";

    pausa();
}

// ============================================================
//  DROPS POR TIPO DE MONSTRO
// ============================================================

int xpParaNivel(int nivel);  // forward declaration

void processarDrops(Jogador& j, const Monstro& m) {

    auto ofereceArma = [&](const string& nome) {
        cout << "Drop de arma: " << nome << "!\n";
        cout << "Equipar? (1-Sim / 0-Nao): ";
        int op; cin >> op;
        if (cin.fail()) { cin.clear(); cin.ignore(1000,'\n'); op = 0; }
        if (op == 1) {
            j.armaEquipada = getArma(nome);
            float np = min(0.99f, j.precisao + j.armaEquipada.bonusPrecisao);
            float nc = min(0.70f, j.chanceCritico + j.armaEquipada.bonusCritico);
            j.precisao = np; j.chanceCritico = nc;
        }
    };

    // Aranha: nada
    if (m.nome == "Aranha") return;

    // Zumbi / Esqueleto
    if (m.nome == "Zumbi" || m.nome == "Esqueleto") {
        if (rand() % 100 < 30) {
            vector<string> drops = {"Pao","Vinho","Espada de Madeira","Arco"};
            string d = drops[rand() % drops.size()];
            if (d == "Pao")   { j.paes++;   cout << "Drop: Pao!\n"; }
            else if (d == "Vinho") { j.vinhos++; cout << "Drop: Vinho!\n"; }
            else ofereceArma(d);
        }
        return;
    }

    // Mago inimigo
    if (m.nome == "Mago") {
        if (rand() % 100 < 35) {
            vector<string> drops = {"Cajado de Fogo","Cajado de Gelo","Pocao","Bife"};
            string d = drops[rand() % drops.size()];
            if (d == "Pocao") { j.pocoes++; cout << "Drop: Pocao!\n"; }
            else if (d == "Bife") { j.bifes++; cout << "Drop: Bife!\n"; }
            else ofereceArma(d);
        }
        return;
    }

    // Golem
    if (m.nome == "Golem") {
        if (rand() % 100 < 40) {
            int r = rand() % 5;
            if (r == 0) ofereceArma("Espada de Ferro");
            else if (r == 1) ofereceArma("Machado");
            else if (r == 2) { j.pocoes++; cout << "Drop: Pocao!\n"; }
            else if (r == 3) { j.bifes++;  cout << "Drop: Bife!\n"; }
            else { int g = 30 + rand() % 50; j.gold += g; cout << "Drop: " << g << " Gold extra!\n"; }
        }
        return;
    }

    // Dragao: drops raros e valiosos
    if (m.nome == "Dragao") {
        cout << "O Dragao deixou cair tesouros!\n";
        int g = 200 + rand() % 300;
        j.gold += g;
        cout << "+" << g << " Gold extra!\n";
        if (rand() % 100 < 40) {
            // chance de espada do infinito
            if (rand() % 100 < 25) ofereceArma("Espada do Infinito");
            else {
                vector<string> raras = {"Cajado das Sombras","Machado","Besta"};
                ofereceArma(raras[rand() % raras.size()]);
            }
        }
        if (rand() % 100 < 50) { j.pocoes++; cout << "Drop: Pocao!\n"; }
        return;
    }

    // generico
    if (rand() % 100 < 20) {
        vector<string> drops = {"Pao","Vinho","Bife","Pocao"};
        string d = drops[rand() % drops.size()];
        if (d == "Pao")   { j.paes++;   cout << "Drop: Pao!\n"; }
        else if (d == "Vinho") { j.vinhos++; cout << "Drop: Vinho!\n"; }
        else if (d == "Bife")  { j.bifes++;  cout << "Drop: Bife!\n"; }
        else { j.pocoes++; cout << "Drop: Pocao!\n"; }
    }
}

// ============================================================
//  XP ESCALAVEL
// ============================================================

int xpParaNivel(int nivel) {
    return (int)(100 * pow(1.25, nivel - 1));
}

// ============================================================
//  APLICAR EFEITO DE TIPO DE DANO (jogador atacando)
// ============================================================

void aplicarEfeitoDano(Jogador& j, Monstro& m, TipoDano tipo, int danoBase) {
    float chancaGelo = 0.20f + (float)(rand() % 16) / 100.0f; // 20-35%

    switch (tipo) {
        case TipoDano::FOGO:
            // dano continuo: 15% do dano por turno por 3 turnos
            m.turnosFogoInimigo = 3;
            m.danoPorTurnoFogoInimigo = max(1, danoBase * 15 / 100);
            cout << "Chamas! O inimigo queimara por " << m.danoPorTurnoFogoInimigo
                 << " dano nos proximos " << m.turnosFogoInimigo << " turnos!\n";
            break;
        case TipoDano::GELO:
            if ((float)(rand() % 100) / 100.0f < chancaGelo) {
                m.turnosGeloInimigo = 1;
                cout << "O inimigo foi CONGELADO! Perde o proximo ataque!\n";
            }
            break;
        case TipoDano::SOMBRA:
        {
            int roubo = max(1, danoBase * 20 / 100);
            j.vida = min(j.vidaMax, j.vida + roubo);
            cout << "Sombra roubou " << roubo << " de vida do inimigo!\n";
        }
            break;
        case TipoDano::INFINITO:
        {
            // alterna fogo -> gelo -> sombra
            if (j.cicloInfinito == 0) {
                m.turnosFogoInimigo = 3;
                m.danoPorTurnoFogoInimigo = max(1, danoBase * 20 / 100);
                cout << "[Infinito-Fogo] Queimadura por " << m.danoPorTurnoFogoInimigo << " x3 turnos!\n";
            } else if (j.cicloInfinito == 1) {
                m.turnosGeloInimigo = 1;
                cout << "[Infinito-Gelo] Inimigo CONGELADO!\n";
            } else {
                int roubo = max(1, danoBase * 25 / 100);
                j.vida = min(j.vidaMax, j.vida + roubo);
                cout << "[Infinito-Sombra] Roubou " << roubo << " de vida!\n";
            }
            j.cicloInfinito = (j.cicloInfinito + 1) % 3;
        }
            break;
        default: break;
    }
}

// ============================================================
//  GERAR MONSTRO
// ============================================================

Monstro gerarMonstro(int nivel) {
    int e = nivel - 1;
    int r = rand() % 100;
    Monstro m;
    m.turnosFogoInimigo = 0; m.danoPorTurnoFogoInimigo = 0;
    m.turnosGeloInimigo = 0; m.ehBoss = false;

    if (r < 1) {
        m = {"Dragao", 200+e*20, 200+e*20, 45+e*3, 100+e*10, 200+e*15, TipoMonstro::ELITE, 0, true, 0,0,0};
    } else if (r < 7) {
        m = {"Golem",  150+e*10, 150+e*10, 30+e*2, 60+e*5,   80+e*8,   TipoMonstro::GUERREIRO_INIMIGO, 0, false, 0,0,0};
    } else if (r < 15) {
        m = {"Mago",   75+e*5,   75+e*5,   20+e,   45+e*4,   50+e*5,   TipoMonstro::MAGO_INIMIGO, 30, false, 0,0,0};
    } else if (r < 35) {
        m = {"Zumbi",  55+e*4,   55+e*4,   12+e,   25+e*3,   25+e*3,   TipoMonstro::NORMAL, 0, false, 0,0,0};
    } else if (r < 65) {
        m = {"Esqueleto", 35+e*3, 35+e*3,  8+e,    18+e*2,   18+e*2,   TipoMonstro::NORMAL, 0, false, 0,0,0};
    } else {
        m = {"Aranha", 20+e*2,   20+e*2,   5+e,    10+e,     10+e,     TipoMonstro::NORMAL, 0, false, 0,0,0};
    }
    return m;
}

// ============================================================
//  BOSS RARO (Dragao especial)
// ============================================================

Monstro gerarBossRaro(int nivel) {
    int e = nivel - 1;
    Monstro m;
    m.nome  = "Dragao";
    m.vidaMax = m.vida = 400 + e * 30;
    m.ataque  = 70 + e * 5;
    m.xp      = 300 + e * 20;
    m.gold    = 500 + e * 30;
    m.tipo    = TipoMonstro::ELITE;
    m.mana    = 0;
    m.ehBoss  = true;
    m.turnosFogoInimigo = 0; m.danoPorTurnoFogoInimigo = 0;
    m.turnosGeloInimigo = 0;
    return m;
}

// ============================================================
//  COMBATE
// ============================================================

void lutar(Jogador& j, bool forcaBoss = false) {
    Monstro m;
    if (forcaBoss) {
        m = gerarBossRaro(j.nivel);
        system("cls");
        cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        cout << "  Voce encontrou um Boss raro!\n";
        cout << "  *** DRAGAO LENDARIO ***\n";
        cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        pausa();
    } else {
        m = gerarMonstro(j.nivel);
        system("cls");
        cout << "Um " << m.nome << " apareceu!\n";
        pausa();
    }

    // ciclo de dano nos tipos de dano ja existentes no monstro
    int cicloTipoBoss = 0; // 0=fogo,1=gelo,2=sombra

    while (j.vida > 0 && m.vida > 0) {
        system("cls");
        cout << "======= COMBATE =======\n";
        cout << "Sua Vida : " << barraVida(j.vida, j.vidaMax)
             << " " << j.vida << "/" << j.vidaMax << "\n";
        cout << m.nome << (m.ehBoss?" [BOSS]":"") << " : "
             << barraVida(m.vida, m.vidaMax)
             << " " << m.vida << "/" << m.vidaMax << "\n\n";
        cout << "Arma: " << j.armaEquipada.nome
             << " [" << nomeTipo(j.armaEquipada.tipo) << "]\n";
        if (j.turnosCongelado > 0)
            cout << "[VOCE ESTA CONGELADO - perde ataque este turno]\n";
        if (j.turnosFogo > 0)
            cout << "[Queimando: " << j.danoPorTurnoFogo << " dano por turno, "
                 << j.turnosFogo << " turnos restantes]\n";
        cout << "\n1 - Atacar\n2 - Usar Item\n3 - Fugir\nOpcao: ";

        int op; cin >> op;
        if (cin.fail()) { cin.clear(); cin.ignore(1000,'\n');
            cout << "\nOpcao invalida!\n"; pausa(); continue; }

        if (op == 1) {
            if (j.turnosCongelado > 0) {
                cout << "\nVoce esta congelado e nao pode atacar!\n";
                j.turnosCongelado--;
            } else {
                // chance de erro
                float roll = (float)(rand() % 100) / 100.0f;
                if (roll > j.precisao) {
                    cout << "\nVoce errou o ataque!\n";
                } else {
                    int dano = j.armaEquipada.danoBase;
                    int var  = rand() % 41 - 20;
                    dano += dano * var / 100;
                    dano = max(1, dano);

                    // critico
                    bool crit = ((float)(rand() % 100) / 100.0f) < j.chanceCritico;
                    if (crit) dano = (int)(dano * j.multiplicadorCritico);

                    // bonus contextual
                    if (j.armaEquipada.tipo == TipoDano::FOGO &&
                        (m.tipo == TipoMonstro::NORMAL || m.tipo == TipoMonstro::ELITE))
                        dano = (int)(dano * 1.15f);
                    if (j.armaEquipada.tipo == TipoDano::GELO &&
                        m.tipo == TipoMonstro::GUERREIRO_INIMIGO)
                        dano = (int)(dano * 1.20f);
                    if (j.armaEquipada.tipo == TipoDano::SOMBRA &&
                        m.tipo == TipoMonstro::MAGO_INIMIGO)
                        dano = (int)(dano * 1.25f);

                    m.vida -= dano;
                    cout << "\nVoce causou " << dano << " de dano ["
                         << nomeTipo(j.armaEquipada.tipo) << "]";
                    if (crit) cout << " CRITICO!";
                    cout << "\n";

                    // efeitos especiais do tipo de dano
                    if (m.vida > 0)
                        aplicarEfeitoDano(j, m, j.armaEquipada.tipo, dano);
                }
            }

            // dano continuo de fogo no jogador
            if (j.turnosFogo > 0) {
                j.vida -= j.danoPorTurnoFogo;
                cout << "Voce esta queimando! -" << j.danoPorTurnoFogo << " vida\n";
                j.turnosFogo--;
            }

            // dano continuo de fogo no monstro
            if (m.turnosFogoInimigo > 0 && m.vida > 0) {
                m.vida -= m.danoPorTurnoFogoInimigo;
                cout << m.nome << " queima! -" << m.danoPorTurnoFogoInimigo << " vida\n";
                m.turnosFogoInimigo--;
            }

            if (m.vida > 0) {
                // IA do monstro
                bool monstroCongelado = (m.turnosGeloInimigo > 0);
                if (monstroCongelado) {
                    cout << m.nome << " esta congelado e nao ataca!\n";
                    m.turnosGeloInimigo--;
                } else if (m.tipo == TipoMonstro::MAGO_INIMIGO &&
                           m.mana >= 20 && m.vida < m.vidaMax / 2) {
                    int cura = 20 + rand() % 20;
                    m.vida = min(m.vidaMax, m.vida + cura);
                    m.mana -= 20;
                    cout << m.nome << " se curou em " << cura << " pontos!\n";
                } else if (m.tipo == TipoMonstro::ELITE && rand() % 100 < 30) {
                    int dExtra = m.ataque + rand() % 10;
                    // boss usa tipos de dano variados
                    if (m.ehBoss) {
                        if (cicloTipoBoss == 0) {
                            // fogo
                            j.turnosFogo = 2;
                            j.danoPorTurnoFogo = max(1, dExtra * 15 / 100);
                            cout << m.nome << " usou Sopro de Fogo: " << dExtra
                                 << " dano + queimadura!\n";
                        } else if (cicloTipoBoss == 1) {
                            // gelo
                            j.turnosCongelado = 1;
                            cout << m.nome << " usou Rajada de Gelo: " << dExtra
                                 << " dano + CONGELADO!\n";
                        } else {
                            // sombra: dreno de vida
                            m.vida = min(m.vidaMax, m.vida + dExtra * 15 / 100);
                            cout << m.nome << " usou Toque das Sombras: " << dExtra
                                 << " dano + drenagem de vida!\n";
                        }
                        cicloTipoBoss = (cicloTipoBoss + 1) % 3;
                    } else {
                        cout << m.nome << " usou Ataque Poderoso: " << dExtra << " de dano!\n";
                    }
                    j.vida -= dExtra;
                } else {
                    j.vida -= m.ataque;
                    cout << m.nome << " causou " << m.ataque << " de dano.\n";
                }

            }
            pausa();
        }
        else if (op == 2) usarItem(j);
        else if (op == 3) {
            // fuga mais dificil
            int chancaFuga = 30; // 30% de sucesso
            if (rand() % 100 < chancaFuga) {
                cout << "\nVoce conseguiu fugir!\n";
                pausa(); return;
            } else {
                int danoFuga = m.ataque + rand() % (m.ataque / 2 + 1);
                j.vida -= danoFuga;
                cout << "\nFuga falhou! Voce sofreu " << danoFuga << " de dano ao tentar escapar!\n";
                pausa();
            }
        } else { cout << "\nOpcao invalida!\n"; pausa(); }
    }

    if (j.vida > 0) {
        system("cls");
        cout << "\nVoce derrotou " << m.nome << (m.ehBoss?" [BOSS LENDARIO]":"") << "!\n";

        int g = (int)(m.gold * j.bonusGold);
        cout << "Ganhou " << m.xp << " XP e " << g << " Gold";
        if (j.classe == ClassePersonagem::LADINO) cout << " (bonus Ladino!)";
        cout << "\n";

        j.xp += m.xp;
        j.gold += g;
        j.monstrosMortos++;
        // limpa efeitos de combate
        j.turnosFogo = 0; j.turnosCongelado = 0;

        processarDrops(j, m);

        // Level up
        while (j.xp >= j.xpProximoNivel) {
            j.xp -= j.xpProximoNivel;
            j.nivel++;
            int vBonus = (j.classe == ClassePersonagem::GUERREIRO) ? 25 : 12;
            int aBonus = (j.classe == ClassePersonagem::MAGO)      ? 3  : 1;
            j.vidaMax += vBonus;
            j.ataque  += aBonus;
            j.precisao      = min(0.99f, j.precisao      + 0.01f);
            j.chanceCritico = min(0.70f, j.chanceCritico + 0.005f);
            j.vida = j.vidaMax;
            j.xpProximoNivel = xpParaNivel(j.nivel);
            cout << "** SUBIU PARA O NIVEL " << j.nivel << "! **\n";
            cout << "+" << vBonus << " VidaMax | +" << aBonus << " Ataque\n";
        }
        pausa();
    }

    if (j.vida <= 0) {
        cout << "\nITS OVER\n";
        pausa();
        exit(0);
    }
}

// ============================================================
//  EVENTOS ALEATORIOS (menu principal, 5-10%)
// ============================================================

void eventoBau(Jogador& j) {
    system("cls");
    cout << "Voce encontrou um bau misterioso!\n\n";
    int r = rand() % 5;
    if (r == 0) {
        int g = 80 + rand() % 150;
        j.gold += g;
        cout << "Dentro havia " << g << " gold!\n";
    } else if (r == 1) {
        j.pocoes++;
        cout << "Dentro havia uma Pocao!\n";
    } else if (r == 2) {
        vector<string> raras = {"Cajado de Gelo","Machado","Arco","Adaga"};
        string a = raras[rand() % raras.size()];
        cout << "Voce encontrou: " << a << "!\n";
        cout << "Equipar? (1-Sim / 0-Nao): ";
        int op; cin >> op;
        if (cin.fail()) { cin.clear(); cin.ignore(1000,'\n'); op = 0; }
        if (op == 1) {
            j.armaEquipada = getArma(a);
            float np = min(0.99f, j.precisao + j.armaEquipada.bonusPrecisao);
            float nc = min(0.70f, j.chanceCritico + j.armaEquipada.bonusCritico);
            j.precisao = np; j.chanceCritico = nc;
        }
    } else if (r == 3) {
        j.bifes++;
        cout << "Dentro havia um Bife!\n";
    } else {
        cout << "O bau estava vazio...\n";
    }
    pausa();
}

void eventoArmadilha(Jogador& j) {
    system("cls");
    cout << "Você caiu em uma armadilha!\n";
    pausa();

    // gera um monstro aleatório baseado no nível do jogador
    Monstro m = gerarMonstro(j.nivel);

    cout << "Da armadilha surge um " << m.nome << "!\n";
    pausa();

    // combate direto contra o monstro gerado
    lutar(j);
}

void eventoAleatorio(Jogador& j) {
    // chance 5-10%
    int chance = 5 + rand() % 6; // 5 a 10
    if (rand() % 100 >= chance) return;

    int r = rand() % 10;
    if (r < 4)       eventoBau(j);
    else if (r < 7)  eventoArmadilha(j);
    else if (r < 10) comercianteRaro(j);
}

// ============================================================
//  SAVE / LOAD
// ============================================================

void salvarJogo(const Jogador& j) {
    ofstream f("savegame.txt");
    if (!f) { cout << "Erro ao salvar!\n"; pausa(); return; }
    f << j.vida            << "\n" << j.vidaMax        << "\n"
      << j.ataque          << "\n" << j.xp             << "\n"
      << j.xpProximoNivel  << "\n" << j.nivel          << "\n"
      << j.gold            << "\n" << j.pocoes          << "\n"
      << j.bifes           << "\n" << j.paes            << "\n"
      << j.vinhos          << "\n" << j.monstrosMortos  << "\n"
      << (int)j.classe     << "\n" << j.nomeClasse      << "\n"
      << j.precisao        << "\n" << j.chanceCritico   << "\n"
      << j.multiplicadorCritico << "\n"
      << j.bonusGold       << "\n"
      << j.cicloInfinito   << "\n"
      << j.armaEquipada.nome << "\n";
    f.close();
    cout << "Jogo salvo!\n";
    pausa();
}

bool carregarJogo(Jogador& j) {
    ifstream f("savegame.txt");
    if (!f) return false;
    int cl;
    string nomeArma;
    f >> j.vida >> j.vidaMax >> j.ataque >> j.xp
      >> j.xpProximoNivel >> j.nivel >> j.gold
      >> j.pocoes >> j.bifes >> j.paes >> j.vinhos
      >> j.monstrosMortos >> cl >> j.nomeClasse
      >> j.precisao >> j.chanceCritico >> j.multiplicadorCritico
      >> j.bonusGold >> j.cicloInfinito
      >> nomeArma;
    j.classe = (ClassePersonagem)cl;
    j.armaEquipada = getArma(nomeArma);
    j.turnosFogo = 0; j.danoPorTurnoFogo = 0;
    j.turnosCongelado = 0; j.temRouboDeVida = false;
    f.close();
    return true;
}

// ============================================================
//  MAIN
// ============================================================

int main() {
    srand((unsigned)time(0));

    cout << "=====================================\n";
    cout << "==== CEMITERIO SEM FIM - RPG ======\n";
    cout << "=====================================\n\n";

    Jogador j;
    j.pocoes = 1; j.bifes = 1; j.paes = 0; j.vinhos = 0;
    j.xp = 0; j.nivel = 1; j.gold = 50; j.monstrosMortos = 0;
    j.turnosFogo = 0; j.danoPorTurnoFogo = 0;
    j.turnosCongelado = 0; j.temRouboDeVida = false;
    j.cicloInfinito = 0;
    j.xpProximoNivel = xpParaNivel(1);

    cout << "1 - Novo Jogo\n2 - Continuar\nOpcao: ";
    int mi; cin >> mi;
    if (cin.fail()) { cin.clear(); cin.ignore(1000,'\n'); mi = 1; }

    if (mi == 2 && carregarJogo(j)) {
        cout << "\nJogo carregado! Bem-vindo de volta, " << j.nomeClasse << "!\n";
        pausa();
    } else {
        cout << "\nHa muito tempo, este cemiterio foi amaldicoado.\n";
        cout << "Os mortos nunca descansam.\n";
        cout << "Criaturas surgem todas as noite.\n";
        cout << "Seu objetivo nao e salvar o mundo.\n";
        cout << "E apenas sobreviver.\n\n";
        pausa();
        escolherClasse(j);
        cout << "\nClasse escolhida: " << j.nomeClasse << "! Boa sorte...\n";
        pausa();
    }

    while (true) {
        system("cls");
        cout << "====================================================\n";
        cout << " Nivel " << j.nivel
             << " | HP " << barraVida(j.vida, j.vidaMax, 15)
             << " " << j.vida << "/" << j.vidaMax
             << " | Gold " << j.gold << "\n";
        cout << "====================================================\n";
        cout << "1 - Batalhar\n";
        cout << "2 - Status\n";
        cout << "3 - Inventario\n";
        cout << "4 - Usar Item\n";
        cout << "5 - Loja\n";
        cout << "6 - Salvar\n";
        cout << "0 - Sair\nOpcao: ";

        int op; cin >> op;
        if (cin.fail()) { cin.clear(); cin.ignore(1000,'\n');
            cout << "\nOpcao invalida!\n"; pausa(); continue; }

        if (op == 0) break;

        if (op == 1) {
            // verifica boss raro (3%)
            bool boss = (rand() % 100 < 3);
            lutar(j, boss);
            // evento aleatorio apos batalha (5-10%)
            eventoAleatorio(j);
        }
        else if (op == 2) status(j);
        else if (op == 3) inventario(j);
        else if (op == 4) usarItem(j);
        else if (op == 5) loja(j);
        else if (op == 6) salvarJogo(j);
    }

    return 0;
}
