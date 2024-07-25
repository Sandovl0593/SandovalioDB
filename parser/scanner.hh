#ifndef SCANNER_HH
#define SCANNER_HH

#include <iostream>
#include <stdlib.h>
#include <unordered_map>
using namespace std;

enum TokenType { 
  LPAREN=0, RPAREN, COMMA, PTCOMMA, EQUAL, NEQ, LT, GT, LTEQ, GTEQ, ALL, ERR, END,
  CREATE, INSERT, INTO, VALUES, UPDATE, DELETE, TABLE, 
  SELECT, FROM, WHERE, ON, SET, BETWEEN, IN, LIKE, AND, OR, NULLW, NOT, AS, LIMIT,
  JOIN, INNER, LEFT, RIGHT, CROSS,
  ID, VARCHAR, INT, FLOAT, BOOLEAN, DATE, TRUE, FALSE
};
const char* token_names[47] = { "(", ")", ",", ";", "=", "<>", "<", ">", "<=", ">=", "ALL", "ERR", "END",
  "CREATE", "INSERT", "INTO", "VALUES", "UPDATE", "DELETE", "TABLE", 
  "SELECT", "FROM", "WHERE", "ON", "SET", "BETWEEN", "IN", "LIKE", "AND", "OR", "NULL", "NOT", "AS", "LIMIT",
  "JOIN", "INNER", "LEFT", "RIGHT", "CROSS",
  "ID", "VARCHAR", "INT", "FLOAT", "BOOLEAN", "DATE", "TRUE", "FALSE"
 };


class Token {
public:
  TokenType type;
  string lexema;
  char error;
  Token(TokenType t);
  Token(TokenType t, string source);
  Token(TokenType t, char err_c);
  ~Token();
};

ostream& operator<<(ostream& os, const Token* value);


class Scanner {
public:
  Scanner();
  Scanner(string in_s);
  Scanner(const char* in_s);
  // Retorna el siguiente token
  Token* nextToken();
  // Reinicia el scanner
  void resetScanner();
  // Cambia el input del scanner
  void setInput(string new_s);
  ~Scanner();

private:
  string input;
  int first, current;
  unordered_map<string, TokenType> reserved;
  void initReserved();
  // Retorna el siguiente caracter del input y avanza el puntero
  char nextChar();
  // retrocede current una posicion
  void rollBack();
// guarda la posicion del puntero actual first para el inicio de un lexema
  void startLexema();
  // retorna el lexema actual
  string getLexema();
  // verifica si un lexema es una palabra reservada
  Token* checkReserved(string lexema);
};

#endif