#include "parser.h"
#include "error.h"
#include <iostream>
#include <sstream>
#include <cctype>
using std::cin;
using std::cout;
using std::endl;
using std::stringstream;

extern yyFlexLexer * scanner;

Statement * Parser::Program()
{
    // program -> int main() block
    if (!Match(Tag::INTEGER))
        throw SyntaxError(scanner->lineno(), "\'int\' esperado");

    if (!Match(Tag::MAIN))
        throw SyntaxError(scanner->lineno(), "\'main\' esperado");

    if (!Match('('))
        throw SyntaxError(scanner->lineno(), "\'(\' esperado");

    if (!Match(')'))
        throw SyntaxError(scanner->lineno(), "\')\' esperado");

    return Block();
}

Statement * Parser::Block()
{
    // block -> { decls stmts }
    if (!Match('{'))
        throw SyntaxError(scanner->lineno(), "\'{\' esperado");

    // ------------------------------------
    // nova tabela de simbolos para o bloco
    // ------------------------------------
    SymTable *saved = symtable;
    symtable = new SymTable(symtable);
    // ------------------------------------

    Decls();
    Statement * sts = Stmts();

    if (!Match('}'))
        throw SyntaxError(scanner->lineno(), "\'}\' esperado");

    // ------------------------------------------------------
    // tabela do escopo envolvente volta a ser a tabela ativa
    // ------------------------------------------------------
    delete symtable;
    symtable = saved;
    // ------------------------------------------------------

    return sts;
}

void Parser::Decls()
{
    // decls -> decl decls
    //        | empty
    // decl  -> type id index;
    //
    // index -> [ integer ]
    //        | empty

    while (lookahead->tag == Tag::INTEGER || lookahead->tag == Tag::FLOATING || lookahead->tag == Tag::BOOLEAN)
    {
        // captura nome do tipo
        string type{lookahead->lexeme};
        if (lookahead->tag == Tag::INTEGER)
        {
            Match(Tag::INTEGER);
        } else if (lookahead->tag == Tag::FLOATING) {
            Match(Tag::FLOATING);
        } else {
            Match(Tag::BOOLEAN);
        }
        

        // captura nome do identificador
        string name{lookahead->lexeme};
        Match(Tag::ID);

        // cria simbolo
        Symbol s{name, type};

        // insere variavel na tabela de simbolos
        if (!symtable->Insert(name, s))
        {
            // a insercao falha quando a variavel ja esta na tabela
            stringstream ss;
            ss << "variavel \"" << name << "\" ja definida";
            throw SyntaxError(scanner->lineno(), ss.str());
        }

        // verifica se eh uma declaracao de arranjo
        if (Match('['))
        {
            if (!Match(Tag::INTEGER))
            {
                stringstream ss;
                ss << "o indice de um arranjo deve ser um valor inteiro";
                throw SyntaxError{scanner->lineno(), ss.str()};
            }
            if (!Match(']'))
            {
                stringstream ss;
                ss << "esperado ] no lugar de  \'" << lookahead->lexeme << "\'";
                throw SyntaxError{scanner->lineno(), ss.str()};
            }
        }

        // verififica ponto e virgula
        if (!Match(';'))
        {
            stringstream ss;
            ss << "encontrado \'" << lookahead->lexeme << "\' no lugar de ';'";
            throw SyntaxError{scanner->lineno(), ss.str()};
        }
    }
}

Statement *Parser::Stmts()
{
    // stmts -> stmt stmts
    //        | empty

    Statement *seq = nullptr;

    switch (lookahead->tag)
    {
    // stmts -> stmt stmts
    case Tag::ID:
    case Tag::IF:
    case Tag::WHILE:
    case Tag::DO:
    case '{':
    {
        Statement *st = Stmt();
        Statement *sts = Stmts();
        seq = new Seq(st, sts);
    }
    }

    // stmts -> empty
    return seq;
}

Statement *Parser::Stmt()
{
    // stmt  -> local = bool;
    //        | if (bool) stmt
    //        | while (bool) stmt
    //        | do stmt while (bool);
    //        | block

    Statement *stmt = nullptr;

    switch (lookahead->tag)
    {
    // stmt -> local = bool;
    case Tag::ID:
    {
        Expression *left = Local();
        if (!Match('='))
        {
            stringstream ss;
            ss << "esperado = no lugar de  \'" << lookahead->lexeme << "\'";
            throw SyntaxError{scanner->lineno(), ss.str()};
        }
        Expression *right = Bool();
        stmt = new Assign(left, right);
        if (!Match(';'))
        {
            stringstream ss;
            ss << "esperado ; no lugar de  \'" << lookahead->lexeme << "\'";
            throw SyntaxError{scanner->lineno(), ss.str()};
        }
        return stmt;
    }

    // stmt -> if (bool) stmt
    case Tag::IF:
    {
        Match(Tag::IF);
        if (!Match('('))
        {
            stringstream ss;
            ss << "esperado ( no lugar de  \'" << lookahead->lexeme << "\'";
            throw SyntaxError{scanner->lineno(), ss.str()};
        }
        Expression *cond = Bool();
        // criacao adiantada do if para pegar erros 
        // da expressao condicional na linha correta
        stmt = new If(cond, nullptr);
        if (!Match(')'))
        {
            stringstream ss;
            ss << "esperado ) no lugar de  \'" << lookahead->lexeme << "\'";
            throw SyntaxError{scanner->lineno(), ss.str()};
        }
        Statement *inst = Stmt();
        // modifica no If que foi criado apenas com a expressao condicional
        ((If*)stmt)->stmt = inst;
        return stmt;
    }

    // stmt -> while (bool) stmt
    case Tag::WHILE:
    {
        Match(Tag::WHILE);
        if (!Match('('))
        {
            stringstream ss;
            ss << "esperado ( no lugar de  \'" << lookahead->lexeme << "\'";
            throw SyntaxError{scanner->lineno(), ss.str()};
        }
        Expression *cond = Bool();
        if (!Match(')'))
        {
            stringstream ss;
            ss << "esperado ) no lugar de  \'" << lookahead->lexeme << "\'";
            throw SyntaxError{scanner->lineno(), ss.str()};
        }
        Statement *inst = Stmt();
        stmt = new While(cond, inst);
        return stmt;
    }

    // stmt -> do stmt while (bool);
    case Tag::DO:
    {
        Match(Tag::DO);
        Statement *inst = Stmt();
        if (!Match(Tag::WHILE))
        {
            stringstream ss;
            ss << "esperado \'while\' no lugar de  \'" << lookahead->lexeme << "\'";
            throw SyntaxError{scanner->lineno(), ss.str()};
        }
        if (!Match('('))
        {
            stringstream ss;
            ss << "esperado ( no lugar de  \'" << lookahead->lexeme << "\'";
            throw SyntaxError{scanner->lineno(), ss.str()};
        }
        Expression *cond = Bool();
        stmt = new DoWhile(inst, cond);
        if (!Match(')'))
        {
            stringstream ss;
            ss << "esperado ) no lugar de  \'" << lookahead->lexeme << "\'";
            throw SyntaxError{scanner->lineno(), ss.str()};
        }
        if (!Match(';'))
        {
            stringstream ss;
            ss << "esperado ; no lugar de  \'" << lookahead->lexeme << "\'";
            throw SyntaxError{scanner->lineno(), ss.str()};
        }
        return stmt;
    }
    // stmt -> block
    case '{':
    {
        stmt = Block();
        return stmt;
    }
    default:
    {
        stringstream ss;
        ss << "\'" << lookahead->lexeme << "\' nao inicia uma instrucao valida";
        throw SyntaxError{scanner->lineno(), ss.str()};
    }
    }
}

Expression *Parser::Local()
{
    // local -> local[bool]
    //        | id

    Expression *expr = nullptr;

    switch (lookahead->tag)
    {
    case Tag::ID:
    {
        // verifica tipo da variavel na tabela de simbolos
        Symbol *s = symtable->Find(lookahead->lexeme);
        if (!s)
        {
            stringstream ss;
            ss << "variavel \"" << lookahead->lexeme << "\" nao declarada";
            throw SyntaxError{scanner->lineno(), ss.str()};
        }

        // identifica o tipo da expressao
        int etype = ExprType::VOID;
        if (s->type == "int")
            etype = ExprType::INT;
        else if (s->type == "float")
            etype = ExprType::FLOAT;
        else if (s->type == "bool")
            etype = ExprType::BOOL;

        // identificador
        expr = new Identifier(etype, new Token{*lookahead});
        Match(Tag::ID);

        // acesso a elemento de um arranjo
        if (Match('['))
        {
            Expression * index = Bool();
            expr = new Access(etype, new Token{Tag::ID, "[]"}, expr, index);

            if (!Match(']'))
            {
                stringstream ss;
                ss << "esperado ] no lugar de  \'" << lookahead->lexeme << "\'";
                throw SyntaxError{scanner->lineno(), ss.str()};
            }
        }
        break;
    }
    default:
    {
        stringstream ss;
        ss << "esperado um local de armazenamento (variavel ou arranjo)";
        throw SyntaxError{scanner->lineno(), ss.str()};
    }
    }

    return expr;
}

Expression *Parser::Bool()
{
    // bool -> join lor
    // lor  -> || join lor
    //       | empty

    Expression *expr1 = Join();

    // funcao Lor()
    while (true)
    {
        Token t = *lookahead;

        if (Match(Tag::OR))
        {
            Expression *expr2 = Join();
            expr1 = new Logical(new Token{t}, expr1, expr2);
        }
        else
        {
            // empty
            break;
        }
    }

    return expr1;
}

Expression *Parser::Join()
{
    // join -> equality land
    // land -> && equality land
    //       | empty

    Expression *expr1 = Equality();

    // funcao Land()
    while (true)
    {
        Token t = *lookahead;
        if (Match(Tag::AND))
        {
            Expression *expr2 = Equality();
            expr1 = new Logical(new Token{t}, expr1, expr2);
        }
        else
        {
            // empty
            break;
        }
    }

    return expr1;
}

Expression *Parser::Equality()
{
    // equality -> rel eqdif
    // eqdif    -> == rel eqdif
    //           | != rel eqdif
    //           | empty

    Expression *expr1 = Rel();

    // funcao Eqdif()
    while (true)
    {
        Token t = *lookahead;

        if (lookahead->tag == Tag::EQ)
        {
            Match(Tag::EQ);
            Expression *expr2 = Rel();
            expr1 = new Relational(new Token{t}, expr1, expr2);
        }
        else if (lookahead->tag == Tag::NEQ)
        {
            Match(Tag::NEQ);
            Expression *expr2 = Rel();
            expr1 = new Relational(new Token{t}, expr1, expr2);
        }
        else
        {
            // empty
            break;
        }
    }

    return expr1;
}

Expression *Parser::Rel()
{
    // rel  -> ari comp
    // comp -> < ari comp
    //       | <= ari comp
    //       | > ari comp
    //       | >= ari comp
    //       | empty

    Expression *expr1 = Ari();

    // funcao Comp()
    while (true)
    {
        Token t = *lookahead;

        if (lookahead->tag == Tag::LT)
        {
            Match(Tag::LT);
            Expression *expr2 = Ari();
            expr1 = new Relational(new Token{t}, expr1, expr2);
        }
        else if (lookahead->tag == Tag::LTE)
        {
            Match(Tag::LTE);
            Expression *expr2 = Ari();
            expr1 = new Relational(new Token{t}, expr1, expr2);
        }
        else if (lookahead->tag == Tag::GT)
        {
            Match(Tag::GT);
            Expression *expr2 = Ari();
            expr1 = new Relational(new Token{t}, expr1, expr2);
        }
        else if (lookahead->tag == Tag::GTE)
        {
            Match(Tag::GTE);
            Expression *expr2 = Ari();
            expr1 = new Relational(new Token{t}, expr1, expr2);
        }
        else
        {
            // empty
            break;
        }
    }

    return expr1;
}

Expression *Parser::Ari()
{
    // ari  -> term oper
    // oper -> + term oper
    //       | - term oper
    //       | empty

    Expression *expr1 = Term();

    // funcao Oper()
    while (true)
    {
        Token t = *lookahead;

        // oper -> + term oper
        if (lookahead->tag == '+')
        {
            Match('+');
            Expression *expr2 = Term();
            expr1 = new Arithmetic(expr1->type, new Token{t}, expr1, expr2);
        }
        // oper -> - term oper
        else if (lookahead->tag == '-')
        {
            Match('-');
            Expression *expr2 = Term();
            expr1 = new Arithmetic(expr1->type, new Token{t}, expr1, expr2);
        }
        // oper -> empty
        else
            break;
    }

    return expr1;
}

Expression *Parser::Term()
{
    // term -> unary calc
    // calc -> * unary calc
    //       | / unary calc
    //       | empty

    Expression *expr1 = Unary();

    // funcao Calc()
    while (true)
    {
        Token t = *lookahead;

        // calc -> * unary calc
        if (lookahead->tag == '*')
        {
            Match('*');
            Expression *expr2 = Unary();
            expr1 = new Arithmetic(expr1->type, new Token{t}, expr1, expr2);
        }
        // calc -> / unary calc
        else if (lookahead->tag == '/')
        {
            Match('/');
            Expression *expr2 = Unary();
            expr1 = new Arithmetic(expr1->type, new Token{t}, expr1, expr2);
        }
        // calc -> empty
        else
            break;
    }

    return expr1;
}

Expression *Parser::Unary()
{
    // unary -> !unary
    //        | -unary
    //        | factor

    Expression *unary = nullptr;

    // unary -> !unary
    if (lookahead->tag == '!')
    {
        Token t = *lookahead;
        Match('!');
        Expression *expr = Unary();
        unary = new UnaryExpr(ExprType::BOOL, new Token{t}, expr);
    }
    // unary -> -unary
    else if (lookahead->tag == '-')
    {
        Token t = *lookahead;
        Match('-');
        Expression *expr = Unary();
        unary = new UnaryExpr(expr->type, new Token{t}, expr);
    }
    else
    {
        unary = Factor();
    }

    return unary;
}

Expression *Parser::Factor()
{
    // factor -> (bool)
    //         | local
    //         | integer
    //         | floating
    //         | true
    //         | false

    Expression *expr = nullptr;

    switch (lookahead->tag)
    {
    // factor -> (bool)
    case '(':
    {
        Match('(');
        expr = Bool();
        if (!Match(')'))
        {
            stringstream ss;
            ss << "esperado ) no lugar de  \'" << lookahead->lexeme << "\'";
            throw SyntaxError{scanner->lineno(), ss.str()};
        }
        break;
    }

    // factor -> local
    case Tag::ID:
    {
        expr = Local();
        break;
    }

    // factor -> integer
    case Tag::INTEGER:
    {
        expr = new Constant(ExprType::INT, new Token{*lookahead});
        Match(Tag::INTEGER);
        break;
    }

    // factor -> floating
    case Tag::FLOATING:
    {
        expr = new Constant(ExprType::FLOAT, new Token{*lookahead});
        Match(Tag::FLOATING);
        break;
    }

    // factor -> true
    case Tag::TRUE:
    {
        expr = new Constant(ExprType::BOOL, new Token{*lookahead});
        Match(Tag::TRUE);
        break;
    }

    // factor -> false
    case Tag::FALSE:
    {
        expr = new Constant(ExprType::BOOL, new Token{*lookahead});
        Match(Tag::FALSE);
        break;
    }

    default:
    {
        stringstream ss;
        ss << "uma expressao eh esperada no lugar de  \'" << lookahead->lexeme << "\'";
        throw SyntaxError{scanner->lineno(), ss.str()};
        break;
    }
    }

    return expr;
}

bool Parser::Match(int tag)
{
    if (tag == lookahead->tag)
    {
        lookahead = new Token {scanner->yylex(), scanner->YYText()};
        return true;
    }

    return false;
}

Parser::Parser()
{
    lookahead = new Token {scanner->yylex(), scanner->YYText()};
    symtable = nullptr;
}

Statement * Parser::Start()
{
    return Program();
}

