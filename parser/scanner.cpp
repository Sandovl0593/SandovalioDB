#include "scanner.hh"
#include <cstring>

// Token section
Token::Token(TokenType t): type(t) {}
Token::Token(TokenType t, string source): type(t), lexema(source) {}
Token::Token(TokenType t, char err_c): type(t), error(err_c) {}

ostream& operator<<(ostream& os, const Token* value) {
  os << token_names[value->type];
  if (!(value->lexema.empty()))
    os << "(" << value->lexema << ")";
  return os;
}

// string uppercase to lowercase
string toLower(string s) {
  string res = s;
  for (int i = 0; i < s.size(); i++)
    if (s[i] >= 'A' && s[i] <= 'Z')
      res[i] = s[i] + 32;
  return res;
}

// Scanner section
void Scanner::initReserved() {
  reserved["create"] = CREATE;
  reserved["insert"] = INSERT;
  reserved["into"] = INTO;
  reserved["values"] = VALUES;
  reserved["update"] = UPDATE;
  reserved["delete"] = DELETE;
  reserved["table"] = TABLE;
  reserved["select"] = SELECT;
  reserved["from"] = FROM;
  reserved["where"] = WHERE;
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
  reserved["join"] = JOIN;
  reserved["inner"] = INNER;
  reserved["left"] = LEFT;
  reserved["right"] = RIGHT;
  reserved["cross"] = CROSS;
  reserved["varchar"] = VARCHAR;
  reserved["int"] = INT;
  reserved["float"] = FLOAT;
  reserved["boolean"] = BOOLEAN;
  reserved["date"] = DATE;
  reserved["true"] = TRUE;
  reserved["false"] = FALSE;
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
  unordered_map<string, TokenType>::const_iterator it = reserved.find(toLower(lexema));
  if (it == reserved.end())
    return new Token(ID, lexema);
  else
    return new Token(it->second);
}

// Retorna el siguiente token del input
Token* Scanner::nextToken() {
  Token* token;
  char c;
  c = nextChar();
  // consume espacios en blanco
  while (c == ' ') c = nextChar();

  // si llega al final del input, retorna un token de fin
  if (c == '\0') return new Token(END);
  startLexema();

  if (c == '\'') {
    // si encuentra una comilla simple, se valida un varchar
    c = nextChar();
    while (c != '\'')  c = nextChar();
    string lex = getLexema();
    token = new Token(VARCHAR, lex.substr(1,lex.size()-2));
  }
  else if (c == '\"') {
    // si encuentra una comilla doble, se valida un varchar
    c = nextChar();
    while (c != '\"')  c = nextChar();
    string lex = getLexema();
    token = new Token(VARCHAR, lex.substr(1,lex.size()-2));
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
        token = new Token(FLOAT, getLexema());
      } else {
        token = new Token(ERR, c);
      }
    } else {
      // caso contrario, es un entero
      rollBack();
      token = new Token(INT, getLexema());
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
      case '(': token = new Token(LPAREN); break;
      case ')': token = new Token(RPAREN); break;
      case '*': token = new Token(ALL); break;
      case ',': token = new Token(COMMA); break;
      case ';': token = new Token(PTCOMMA); break;
      case '=': token = new Token(EQUAL); break;
      case '<':
        c = nextChar();
        if (c == '=')
          token = new Token(LTEQ);
        else if (c == '>') 
          token = new Token(NEQ);
        else {
          rollBack();
          token = new Token(LT);
        }; 
        break;

      case '>':
        c = nextChar();
        if (c == '=')
          token = new Token(GTEQ);
        else {
          rollBack();
          token = new Token(GT);
        }; 
        break;
    }
  } else {
    token = new Token(ERR, c);
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