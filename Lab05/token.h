#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using std::string;

struct Token {
    int tag;
    string lexeme;

    Token() : tag(0) {}
    Token(char ch) : tag(int(ch)), lexeme({ch}) {}
    Token(int t, const char* s) : tag(t), lexeme(s) {}
};

#endif // TOKEN_H
