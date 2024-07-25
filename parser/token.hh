#ifndef TOKEN_HH
#define TOKEN_HH

#include <iostream>

using namespace std;

class Token {
public:
  virtual ~Token() = 0;
};

enum SignType { LPAREN=0, RPAREN, COMMA, PTCOMMA, EQUAL, NEQ, LT, GT, LTEQ, GTEQ, ALL, ERR, END };
const char* sign_names[13] = {
  "(", ")", ",", ";", "=", "!=", "<", ">", "<=", ">=", "*", "ERR", "END"
};

enum KeywordType { CREATE=0, INSERT, INTO, VALUES, UPDATE, DELETE, TABLE, SELECT, FROM, WHERE, ON, SET, BETWEEN, IN, LIKE, AND, OR, NULLW, NOT, AS, LIMIT,
                   JOIN, INNER, LEFT, RIGHT, CROSS };
const char* keyword_names[26] = {
  "CREATE", "INSERT", "INTO", "VALUES", "UPDATE", "DELETE", "TABLE", "SELECT", "FROM", "WHERE", "ON", "SET", "BETWEEN", "IN", "LIKE", "AND", "OR", "NULL", "NOT", "AS", "LIMIT",
  "JOIN", "INNER", "LEFT", "RIGHT", "CROSS"
};

enum ValueType { ID=0, VARCHAR, INT, FLOAT, BOOLEAN, DATE };
const char* value_names[6] = {
  "ID", "VARCHAR", "INT", "FLOAT", "BOOLEAN", "DATE"
};


// Signs
class Sign: public Token {
public:
  SignType type;
  static const char* sign_names[12];
  char error;
  Sign(SignType t);
  Sign(SignType, char err_c);
  ~Sign() {}
};

ostream& operator<<(ostream& os, const Sign* sign);

// Keywords tokens SQL
class Keyword: public Token {
public:
  KeywordType type;
  static const char* keyword_names[25];
  Keyword(KeywordType t);
  ~Keyword() {}
};

ostream& operator<<(ostream& os, const Keyword* keyword);

// Values tokens
class Value: public Token {
public:
  ValueType type;
  string lexema;
  static const char* value_names[6];
  Value(ValueType t);
  Value(ValueType, const string source);
  ~Value() {}
};

ostream& operator<<(ostream& os, const Value* value);

#endif