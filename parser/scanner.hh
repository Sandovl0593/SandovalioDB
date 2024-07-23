#ifndef SCANNER_HH
#define SCANNER_HH

#include <cstring>
#include <sstream>
#include <stdlib.h>
#include "token.hh"
#include <unordered_map>


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
  unordered_map<string, KeywordType> reserved;
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