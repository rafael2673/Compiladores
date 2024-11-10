#include "parser.h"
#include "tokens.h"
#include <iostream>
void Parser::Start()
{
    // enquanto não atingir o fim da entrada
    while ((lookahead = scanner.yylex()) != 0)
    {
        // trata o token recebido do analisador léxico
        switch(lookahead)
        {
            case WHILE: cout << "WHILE\n"; break;
            case IF: cout << "IF\n"; break;
            case THEN: cout << "THEN\n"; break;
            case ELSE: cout << "ELSE\n"; break;
            case ID: cout << "ID: " << scanner.YYText() << "\n"; break;
            case NUM: cout << "NUM: " << scanner.YYText() << "\n"; break;
            case RELOP: cout << "RELOP: " << scanner.YYText() << "\n";; break;
        }
    }
}
