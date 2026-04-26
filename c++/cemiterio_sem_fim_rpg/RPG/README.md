# ⚰️ Cemitério Sem Fim — RPG

> *"Seu objetivo não é salvar o mundo. É apenas sobreviver."*

Um RPG de texto por turnos desenvolvido em C++17, jogado inteiramente no terminal. Explore um cemitério amaldiçoado, escolha sua classe, enfrente monstros, acumule gold e sobreviva o máximo possível.

---

## 📋 Índice

- [Compilação e Execução](#compilação-e-execução)
- [Classes Disponíveis](#classes-disponíveis)
- [Sistema de Combate](#sistema-de-combate)
- [Armas e Tipos de Dano](#armas-e-tipos-de-dano)
- [Monstros](#monstros)
- [Loja e Comércio](#loja-e-comércio)
- [Drops de Monstros](#drops-de-monstros)
- [Eventos Aleatórios](#eventos-aleatórios)
- [Sistema de Progressão](#sistema-de-progressão)
- [Save / Load](#save--load)
- [Estrutura do Código](#estrutura-do-código)

---

## 🔧 Compilação e Execução

**Requisitos:** compilador C++ com suporte a C++17 (g++ 7+ ou clang++ 5+)

```bash
# Compilar
g++ -std=c++17 -o rpg rpg.cpp

# Executar
./rpg
```

> **Windows:** o jogo usa `system("cls")` para limpar a tela. Compile normalmente com MinGW ou MSVC.

---

## 🧙 Classes Disponíveis

Ao iniciar um novo jogo, o jogador escolhe uma das 5 classes. Cada uma tem atributos únicos e influencia toda a experiência de jogo.

| Classe     | Vida | Ataque | Precisão | Crítico | Mult. Crítico | Bônus Gold | Arma Inicial       |
|------------|------|--------|----------|---------|---------------|------------|--------------------|
| Guerreiro  | 150  | 15     | 78%      | 12%     | ×1.8          | —          | Espada de Madeira  |
| Assassino  | 80   | 13     | 85%      | 40%     | ×3.0          | —          | Punho              |
| Mago       | 75   | 14     | 68%      | 30%     | ×3.0          | —          | Cajado de Fogo     |
| Ladino     | 90   | 10     | 83%      | 28%     | ×2.5          | +50%       | Punho              |
| Arqueiro   | 90   | 11     | 97%      | 18%     | ×2.0          | —          | Arco               |

**Destaques por classe:**
- **Guerreiro** — máximo de vida e ganho de vida por nível (+25 por level up vs. +12 das demais).
- **Assassino** — críticos devastadores, mas frágil.
- **Mago** — baixa precisão, alto potencial de crítico e bônus de ataque por nível (+3 vs. +1).
- **Ladino** — única classe com bônus de gold (50% a mais em todas as recompensas).
- **Arqueiro** — precisão quase perfeita, raramente erra ataques.

---

## ⚔️ Sistema de Combate

O combate é por turnos. A cada turno, o jogador pode:

| Opção           | Descrição                                                                                        |
|-----------------|--------------------------------------------------------------------------------------------------|
| `1 - Atacar`    | Rola precisão → aplica dano com variação ±20% → verifica crítico → aplica efeito de tipo de dano |
| `2 - Usar Item` | Abre o menu de consumíveis durante o combate                                                     |
| `3 - Fugir`     | 30% de chance de sucesso; falha aplica dano extra do monstro                                     |

**Fórmula de dano:**
```
dano = danoBase ± 20%
se crítico: dano *= multiplicadorCrítico
se bônus de tipo: dano *= (1.15 ~ 1.25)
```

**Efeitos de status:**
- 🔥 **Queimando** — sofre dano contínuo por 2–3 turnos; não pode ser evitado.
- 🧊 **Congelado** — perde o ataque no próximo turno.

**IA dos monstros:**
- Monstros normais atacam com dano fixo.
- **Mago inimigo** se cura quando com menos de 50% de vida (custa 20 de mana).
- **Elite** tem 30% de chance de usar Ataque Poderoso com dano extra.
- **Dragão (Boss)** alterna ciclicamente entre Sopro de Fogo, Rajada de Gelo e Toque das Sombras.

---

## 🗡️ Armas e Tipos de Dano

### Armas disponíveis

| Arma               | Dano | Tipo     | Mult. Crítico | Bônus Precisão | Bônus Crítico | Efeito Especial                          |
|--------------------|------|----------|---------------|----------------|---------------|------------------------------------------|
| Punho              | 5    | Físico   | ×1.5          | —              | +5%           | Arma inicial do Assassino e Ladino       |
| Espada de Madeira  | 10   | Físico   | ×1.5          | —              | —             | Arma inicial do Guerreiro                |
| Espada de Ferro    | 20   | Físico   | ×1.8          | —              | —             | —                                        |
| Adaga              | 22   | Físico   | ×2.8          | —              | +12%          | Alto crítico                             |
| Arco               | 24   | Físico   | ×2.0          | —              | —             | Arma inicial do Arqueiro                 |
| Besta              | 25   | Físico   | ×2.0          | +20%           | —             | Aumenta precisão permanentemente         |
| Machado            | 30   | Físico   | ×2.5          | —              | +10%          | Crítico elevado                          |
| Cajado de Fogo     | 28   | Fogo     | ×2.0          | —              | —             | Queima por 15% do dano/turno × 3 turnos  |
| Cajado de Gelo     | 25   | Gelo     | ×1.9          | —              | —             | 20–35% de chance de congelar             |
| Cajado das Sombras | 35   | Sombra   | ×2.2          | —              | —             | Rouba 20% do dano como vida              |
| Espada do Infinito | 70   | Infinito | ×3.0          | —              | —             | Alterna Fogo → Gelo → Sombra por ciclo   |

### Bônus de tipo contra monstros

| Tipo de Dano | Bônus contra          | Multiplicador |
|--------------|-----------------------|---------------|
| Fogo         | Normal / Elite        | ×1.15         |
| Gelo         | Guerreiro Inimigo     | ×1.20         |
| Sombra       | Mago Inimigo          | ×1.25         |

---

## 👹 Monstros

Os monstros são gerados aleatoriamente com atributos escalados pelo nível do jogador (`nivel - 1` como escala).

| Monstro   | Prob. | Tipo              | Boss | Vida base | Ataque base | XP base | Gold base | Observações                           |
|-----------|-------|-------------------|------|-----------|-------------|---------|-----------|---------------------------------------|
| Aranha    | 35%   | Normal            | Não  | 20        | 5           | 10      | 10        | Inimigo mais fraco; sem drops         |
| Esqueleto | 30%   | Normal            | Não  | 35        | 8           | 18      | 18        | Drops ocasionais                      |
| Zumbi     | 20%   | Normal            | Não  | 55        | 12          | 25      | 25        | Drops ocasionais                      |
| Mago      | 8%    | Mago Inimigo      | Não  | 75        | 20          | 45      | 50        | Se cura; drops de cajados e poções    |
| Golem     | 6%    | Guerreiro Inimigo | Não  | 150       | 30          | 60      | 80        | Drops de armas e poções               |
| Dragão    | 1%    | Elite             | Sim* | 200       | 45          | 100     | 200       | Boss randômico; drops raros valiosos  |

**Boss Raro (Dragão Lendário):** há 3% de chance por batalha de forçar o spawn de um Dragão Boss com stats amplificados e padrões de ataque variados.

---

## 🛒 Loja e Comércio

Acessada pelo menu principal (`5 - Loja`).

### Loja Principal

| Item               | Custo  | Efeito                             |
|--------------------|--------|------------------------------------|
| Espada de Madeira  | 30 g   | Equipa arma                        |
| Espada de Ferro    | 80 g   | Equipa arma                        |
| Adaga              | 120 g  | Equipa arma                        |
| Arco               | 100 g  | Equipa arma                        |
| Cajado de Fogo     | 240 g  | Equipa arma                        |
| Cajado de Gelo     | 200 g  | Equipa arma                        |
| Poção              | 200 g  | Restaura vida total                |
| Bife               | 80 g   | +100 de vida                       |
| Pão                | 40 g   | +50 de vida                        |
| Vinho              | 20 g   | +25 de vida                        |

### Comerciante Raro

Aparece via [evento aleatório](#eventos-aleatórios). Vende itens exclusivos:

| Item               | Custo   | Efeito                                |
|--------------------|---------|---------------------------------------|
| Espada do Infinito | 5000 g  | Arma lendária, ciclo Fogo/Gelo/Sombra |
| Cajado das Sombras | 1000 g  | Roubo de vida                         |
| Machado            | 250 g   | Alto crítico                          |
| Besta              | 120 g   | +20% de precisão permanente           |
| Elixir Supremo     | 500 g   | Cura total + VidaMax +20              |

---

## 🎁 Drops de Monstros

Após cada batalha vitoriosa, há chance de drop conforme o monstro derrotado:

| Monstro   | Chance | Possíveis Drops                                          |
|-----------|--------|----------------------------------------------------------|
| Aranha    | 0%     | Nenhum                                                   |
| Zumbi     | 30%    | Pão, Vinho, Espada de Madeira, Arco                      |
| Esqueleto | 30%    | Pão, Vinho, Espada de Madeira, Arco                      |
| Mago      | 35%    | Cajado de Fogo, Cajado de Gelo, Poção, Bife              |
| Golem     | 40%    | Espada de Ferro, Machado, Poção, Bife, Gold extra (30–80)|
| Dragão    | 100%*  | Gold (200–500), chance de Espada do Infinito ou arma rara, Poção |
| Genérico  | 20%    | Pão, Vinho, Bife, Poção                                  |

---

## 🎲 Eventos Aleatórios

Após cada batalha há 5–10% de chance de um evento aleatório ocorrer:

| Evento              | Prob. | Descrição                                                      |
|---------------------|-------|----------------------------------------------------------------|
| Baú Misterioso      | 40%   | Contém gold, poção, arma rara ou pode estar vazio              |
| Armadilha           | 30%   | Spawn imediato de um monstro aleatório baseado no nível atual  |
| Comerciante Raro    | 30%   | Acesso à loja de itens exclusivos                              |

---

## 📈 Sistema de Progressão

**Level Up:** ao acumular XP suficiente, o jogador sobe de nível automaticamente.

```
XP necessário para o próximo nível = 100 × 1.25^(nível - 1)
```

**Ganhos por nível:**

| Atributo       | Guerreiro | Outras classes   |
|----------------|-----------|------------------|
| Vida Máxima    | +25       | +12              |
| Ataque         | +1        | +1 (+3 para Mago)|
| Precisão       | +1%       | +1% (máx. 99%)   |
| Chance Crítico | +0.5%     | +0.5% (máx. 70%) |

Ao subir de nível, a vida é **totalmente restaurada**.

---

## 💾 Save / Load

O jogo salva automaticamente em `savegame.txt` (texto plano) na opção `6 - Salvar` do menu principal.

**Dados salvos:** vida, vida máxima, ataque, XP, nível, gold, inventário de consumíveis, classe, precisão, chance de crítico, multiplicador de crítico, bônus de gold, arma equipada e ciclo do Infinito.

**Carregar:** ao iniciar o jogo, selecione `2 - Continuar`. Se o arquivo não existir, um novo jogo é iniciado automaticamente.

> Os efeitos de status temporários (queimando, congelado) **não são salvos** e são resetados ao carregar.

---

## 🗂️ Estrutura do Código

```
rpg.cpp
├── Enums          — ClassePersonagem, TipoDano, TipoMonstro
├── Structs        — Arma, Jogador, Monstro
├── Utilitários    — pausa(), barraVida(), nomeTipo()
├── Armas          — getArma()
├── Personagem     — escolherClasse(), status(), inventario(), usarItem()
├── Loja           — loja(), comercianteRaro()
├── Drops          — processarDrops()
├── Progressão     — xpParaNivel()
├── Efeitos        — aplicarEfeitoDano()
├── Monstros       — gerarMonstro(), gerarBossRaro()
├── Combate        — lutar()
├── Eventos        — eventoBau(), eventoArmadilha(), eventoAleatorio()
├── Save/Load      — salvarJogo(), carregarJogo()
└── main()
```

---

## 📄 Licença

Projeto de uso livre. Sinta-se à vontade para modificar, expandir e distribuir.
