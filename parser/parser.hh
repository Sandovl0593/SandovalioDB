#ifndef PARSE_HH
#define PARSE_HH

#include "scanner.hh"
#include "ast.hh"

using namespace std;
class Parser {
private:
    Scanner* scanner;
    Token *current, *previous;
    string report, reportFin;

    bool match(TokenType ttype);
    bool check(TokenType ttype);
    void advance();
    bool isAtEnd();
    void resetParser();
    // void clearMemory();
    void throwParser(string err);

    // Bajo la gramatica SQL
    QueryList* parseQueryList();
    Query* parseQuery();
    CreateQuery* parseCreateQuery();
    SelectQuery* parseSelectQuery();
    InsertQuery* parseInsertQuery();
    UpdateQuery* parseUpdateQuery();
    DeleteQuery* parseDeleteQuery();

    AtributeSent* parseAtributeSent();
    SelectSent* parseSelectSent();
    FromSent* parseFromSent();
    WhereSent* parseWhereSent();
    LimitSent* parseLimitSent();
    UpdateSent* parseUpdateSent();
    JoinSent* parseJoinSent();

    Exp* parseBinaryExp();
    Exp* parseUnaryExp();
    Exp* parseParenExp();
    FilterExp* parseFilterExp();
    BetweenExp* parseBetweenExp();
    LikeExp* parseLikeExp();
    InValueExp* parseInValueExp();
    InQueryExp* parseInQueryExp();
    JoinExp* parseJoinExp();

public:
    Parser(Scanner* scanner);
    void parse();
    string reportParse();
    void setInput(string input);
};

// Gramatica SQL
// <query_list> ::= <query> (';' <query_list>)*
// <query> ::= <create_query> | <insert_query> | <select_query> | <delete_query>
// <create_query> ::= CREATE TABLE <id> '(' <atribute_sent> [',' <atribute_sent>]* ')'
// <select_query> ::= <select_sent> <from_sent> [ <where_sent> ] [ <limit_sent> ]

// <atribute_sent> ::= <id> <type> [NOT NULL]
// <select_sent> ::= SELECT ('*' | <atribute_sent> (',' <atribute_sent>)*)
// <from_sent> ::= FROM <table> [ <join_sent> ]

// <table> ::= ['(' <select_query> ')' AS] <id>

// <join_sent> ::= [ <join_op> ] JOIN <table> ON <join_exp>
// <where_sent> ::= WHERE <exp>
// <limit_sent> ::= LIMIT <int>
// <update_sent> ::= UPDATE <table> SET <id> '=' <value> (',' <id> '=' <value>)*

// <insert_query> ::= INSERT INTO <id> VALUES '(' <value> (',' <value>)* ')'
// <update_query> ::= <update_sent> [ <where_sent> ]
// <delete_query> ::= DELETE FROM <table> WHERE <filter_exp>

// <exp> ::= <binary_exp> | <unary_exp> | <paren_exp> | <filter_exp> | <between_exp> | <like_exp> | <in_value_exp> | <in_query_exp> | <join_exp>
// <binary_exp> ::= <exp> <binary_op> <exp>
// <unary_exp> ::= <unary_op> <exp>
// <paren_exp> ::= '(' <exp> ')'
// <filter_exp> ::= <id> <filter_op> <value>
// <between_exp> ::= <id> BETWEEN <value> AND <value>
// <like_exp> ::= <id> LIKE <value>
// <in_value_exp> ::= <id> IN '(' <value> (',' <value>)* ')'
// <in_query_exp> ::= <id> IN '(' <select_query> ')'
// <join_exp> ::= <id> <binary_op> <id>

// <value> ::= <int> | <float> | <varchar> | <boolean> | <date>
// <filter_op> ::= '=' | '<>' | '<' | '>' | '<=' | '>='
// <unary_op> ::= NOT | '-'
// <binary_op> ::= AND | OR
// <join_op> ::= INNER | LEFT | RIGHT | CROSS
// <type> ::= INT | FLOAT | VARCHAR | BOOLEAN | DATE

#endif