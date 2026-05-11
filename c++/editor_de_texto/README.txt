# 📝 Editor de Texto Básico em C++

Um editor de texto simples feito em C++ para manipulação de arquivos `.txt`.  
Permite escrever, ler e buscar palavras em arquivos, além de contar linhas e caracteres.

## 🚀 Funcionalidades
- Escrever texto em um arquivo (`texto.txt`).
- Ler o conteúdo do arquivo e exibir no console.
- Contar número de linhas e caracteres.
- Buscar palavras específicas dentro do arquivo.

## 📂 Estrutura
- `EditorTexto::escrever()` → adiciona texto ao arquivo.
- `EditorTexto::ler()` → lê e mostra conteúdo, contando linhas e caracteres.
- `EditorTexto::buscarPalavra()` → procura uma palavra e mostra em quais linhas aparece.
- `main()` → menu interativo para o usuário.

## ⚙️ Como compilar
No terminal, use o compilador `g++`:

```bash
g++ -o editor editor.cpp
Isso gera o executável editor.

▶️ Como executar
Após compilar, rode:

bash
./editor
O programa abrirá um menu com opções:

Escrever no arquivo

Ler arquivo

Buscar palavra

Sair

📌 Exemplo de uso
Escolha 1 para escrever no arquivo. Digite linhas de texto e finalize com SAIR.

Escolha 2 para ler o arquivo e ver estatísticas.

Escolha 3 para buscar uma palavra específica.