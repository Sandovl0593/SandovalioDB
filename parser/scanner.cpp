#include "scanner.hh"

void Scanner::initReserved() {
  reserved["create"] = CREATE;
  reserved["insert"] = INSERT;
  reserved["into"] = INTO;
  reserved["values"] = VALUES;
  reserved["update"] = UPDATE;
  reserved["drop"] = DROP;
  reserved["table"] = TABLE;
  reserved["select"] = SELECT;
  reserved["from"] = FROM;
  reserved["where"] = WHERE;
  reserved["if"] = IF;
  reserved["exists"] = EXISTS;
  reserved["on"] = ON;
  reserved["set"] = SET;
  reserved["between"] = BETWEEN;
  reserved["in"] = IN;
  reserved["like"] = LIKE;
  reserved["and"] = AND;
  reserved["or"] = OR;
  reserved["null"] = NULLW;
  reserved["not"] = NOT;
  reserved["as"] = AS;
  reserved["limit"] = LIMIT;
  reserved["using"] = USING;
  reserved["index"] = INDEX;
}

Scanner::Scanner(): input(""), first(0), current(0) {
  initReserved();
}

Scanner::Scanner(string in_s): input(in_s), first(0), current(0) {
  initReserved();
}

Scanner::Scanner(const char* in_s): input(in_s), first(0), current(0) {
  initReserved();
}

void Scanner::setInput(string new_s) {
  this->input = new_s;
}

void Scanner::resetScanner() { first = 0; current = 0; }

// Verifica si el lexema es una palabra reservada
Token* Scanner::checkReserved(string lexema) {
  unordered_map<string, KeywordType>::const_iterator it = reserved.find(lexema);
  if (it == reserved.end())
    return new Value(ID, lexema);
  else
    return new Keyword(it->second);
}

// Retorna el siguiente token del input
Token* Scanner::nextToken() {
  Token* token;
  char c;
  c = nextChar();
  // consume espacios en blanco
  while (c == ' ') c = nextChar();

  // si llega al final del input, retorna un token de fin
  if (c == '\0') return new Sign(END);
  startLexema();

  if (c == '\'') {
    // si encuentra una comilla simple, se valida un varchar
    c = nextChar();
    while (c != '\'')  c = nextChar();
    string lex = getLexema();
    token = new Value(VARCHAR, lex.substr(1,lex.size()-2));
  }
  else if (c == '\"') {
    // si encuentra una comilla doble, se valida un varchar
    c = nextChar();
    while (c != '\"')  c = nextChar();
    string lex = getLexema();
    token = new Value(VARCHAR, lex.substr(1,lex.size()-2));
  }

  else if (isdigit(c)) {
    c = nextChar();
    while (isdigit(c)) c = nextChar();

    if (c == '.') {
      // si encuentra un punto, verifica si es un flotante
      c = nextChar();
      if (isdigit(c)) {
        while (isdigit(c)) c = nextChar();
        rollBack();
        token = new Value(FLOAT, getLexema());
      } else {
        token = new Sign(ERR, c);
      }
    } else {
      // caso contrario, es un entero
      rollBack();
      token = new Value(INT, getLexema());
    }
  } 
  
  else if (isalpha(c) || c == '_') {
    // si encuentra una letra o un guion bajo, valida una palabra reservada o un identificador
    c = nextChar();
    while (isalpha(c) || c == '_')  c = nextChar();
    rollBack();

    string lex = getLexema();
    token = checkReserved(lex);
  } 
  
  else if (strchr("*(),;<>=", c)) {
    // si encuentra un caracter especial, retorna el token correspondiente
    switch(c) {
      case '(': token = new Sign(LPAREN); break;
      case ')': token = new Sign(RPAREN); break;
      case '*': token = new Sign(ALL); break;
      case ',': token = new Sign(COMMA); break;
      case ';': token = new Sign(PTCOMMA); break;
      case '=': token = new Sign(EQUAL); break;
      case '<':
        c = nextChar();
        if (c == '=')
          token = new Sign(LTEQ);
        else {
          rollBack();
          token = new Sign(LT);
        }; 
        break;

      case '>':
        c = nextChar();
        if (c == '=')
          token = new Sign(GTEQ);
        else {
          rollBack();
          token = new Sign(GT);
        }; 
        break;
    }
  } else {
    token = new Sign(ERR, c);
  }
  cout << "next token " << token << endl;
  return token;
}


Scanner::~Scanner() { }

// retorna el siguiente caracter del input y avanza el puntero
char Scanner::nextChar() {
    int c = input[current];
    current++;
    return c;
}

// retrocede current una posicion
void Scanner::rollBack() {
    current--;
}

// guarda la posicion del puntero actual first para el inicio de un lexema
void Scanner::startLexema() {
    first = current-1;
    return;
}

// retorna el lexema actual
string Scanner::getLexema() {
    return input.substr(first,current-first);
}