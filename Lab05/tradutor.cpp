#include <iostream>
#include <fstream>
#include <cstring>
#include "parser.h"
#include "error.h"
#include "gen.h"
#include "checker.h"
#include "FlexLexer.h" // Inclua a definição do FlexLexer

using namespace std;

ifstream fin;
yyFlexLexer *scanner;

int main(int argc, char **argv) {
    if (argc == 2) {
        fin.open(argv[1]);
        if (!fin.is_open()) {
            cout << "Falha na abertura do arquivo \'" << argv[1] << "\'.\n";
            exit(EXIT_FAILURE);
        }

        // Direcionando a entrada do Flex para o ifstream
        yyFlexLexer leitor(&fin); // Passa ifstream para o FlexLexer
        scanner = &leitor;
        Statement *ast;
        Parser tradutor;
        try {
            // gera árvore sintática
            ast = tradutor.Start();
            
            // gera código intermediário
            ast->Gen();
        } catch (SyntaxError err) {
            err.What();
        }
        fin.close();
        // TestParser(ast);        
    }
}
