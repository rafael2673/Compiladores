#include "parser.h"
#include "tokens.h"
#include <iostream>
void Parser::Start()
{
    // enquanto não atingir o fim da entrada
    while ((lookahead = scanner.yylex()) != 0)
    {
        // trata o token recebido do analisador léxico
        switch (lookahead)
        {
        case WHILE:
            cout << "WHILE\n";
            break;
        case IF:
            cout << "IF\n";
            break;
        case THEN:
            cout << "THEN\n";
            break;
        case ELSE:
            cout << "ELSE\n";
            break;
        case ID:
            cout << "ID: " << scanner.YYText() << "\n";
            break;
        case NUM:
            cout << "NUM: " << scanner.YYText() << "\n";
            break;
        case STRING:
        {
            std::string str = scanner.YYText(); // obtém a string reconhecida
            // Remove as aspas iniciais e finais
            str = str.substr(1, str.size() - 2);

            // Substitui sequências de escape
            size_t pos = 0;
            while ((pos = str.find("\\\"", pos)) != std::string::npos)
            {
                str.replace(pos, 2, "\"");
                pos += 1;
            }

            pos = 0;
            while ((pos = str.find("\\\\", pos)) != std::string::npos)
            {
                str.replace(pos, 2, "\\");
                pos += 1;
            }

            cout << "STRING: " << str << "\n";
            break;
        }
        case RELOP:
            cout << "RELOP: " << scanner.YYText() << "\n";
            break;
        }
    }
}
