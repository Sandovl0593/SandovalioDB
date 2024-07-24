#ifndef AST_HH
#define AST_HH

#include <cstring>

#include <list>
#include "imp_type.hh"
#include "imp_value.hh"

using namespace std;

class ImpVisitor;
class TypeVisitor;

enum FilterOp { EQ, NEQ, LT, GT, LTEQ, GTEQ };
enum BinaryOp { AND, OR };
enum UnaryOp { NEG, NOT };

// Toda expresion es un una operacion o valor
class Exp {
public:
  virtual void accept(ImpVisitor* v) = 0;
  virtual ImpType accept(TypeVisitor* v) = 0;
  static string binopToString(BinaryOp op);
  virtual ~Exp() = 0;
};


// Expresion de la forma: <left> <op> <right>
class BinaryExp : public Exp {
public:
  Exp *left;
  Exp *right;
  BinaryOp op;
  BinaryExp(Exp* l, Exp* r, BinaryOp op);
  void accept(ImpVisitor* v);
  ImpType accept(TypeVisitor* v);
  ~BinaryExp();
};


// Expresion de filtro de la forma: <id> <op> <comp>
class FilterExp : public Exp {
public:
  string id;
  Exp *comp;
  FilterOp op;
  FilterExp(string id, Exp* comp, FilterOp op);
  void accept(ImpVisitor* v);
  ImpType accept(TypeVisitor* v);
  ~FilterExp();
};


// Expresion con Between de la forma: <id> BETWEEN <left> AND <right>
class BetweenExp : public Exp {
public:
  string id;
  Exp *left;
  Exp *right;
  BetweenExp(string id, Exp* l, Exp* r);
  void accept(ImpVisitor* v);
  ImpType accept(TypeVisitor* v);
  ~BetweenExp();
};


// Expresion con Like de la forma: <id> LIKE <pattern>
class LikeExp : public Exp {
public:
  string id;
  string pattern;
  LikeExp(string id, string pattern);
  void accept(ImpVisitor* v);
  ImpType accept(TypeVisitor* v);
  ~LikeExp();
};


// Expresion de la forma: <value> que puede ser <int>, <varchar>, <boolean>, <date>
class ValueExp : public Exp {
public:
  ImpValue value;
  ValueExp(int v);
  void accept(ImpVisitor* v);
  ImpType accept(TypeVisitor* v);
  ~ValueExp();
};


// Expresion de la forma: '(' <e> ')'
class ParenthExp : public Exp {
public:
  Exp *e;
  ParenthExp(Exp *e);
  void accept(ImpVisitor* v);
  ImpType accept(TypeVisitor* v);
  ~ParenthExp();
};


// Expresion de la forma: <id>
class IdExp : public Exp {
public:
  string id;
  IdExp(string id);
  void accept(ImpVisitor* v);
  ImpType accept(TypeVisitor* v);
  ~IdExp();
};




// Un sentence es una instruccion interna dentro de uma consulta SQL
class Sentence {
public:
  virtual void accept(ImpVisitor* v) = 0;
  virtual void accept(TypeVisitor* v) = 0;
  virtual ~Sentence() = 0;
};


// Atribute Sentence de la forma:
// <id> <type> [NOT NULL]
class AtributeSent: public Sentence {
public:
  string id;
  ImpType type;
  bool not_null = false;
  AtributeSent(string id, ImpType type, bool not_null);
  void accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~AtributeSent();
};


// Create Sentence de la forma:
// CREATE TABLE <table> '(' <atribute> [',' <atribute>]* ')'
class CreateSent: public Sentence {
public:
  string table;
  list<AtributeSent*> atributes;
  CreateSent(string table, list<AtributeSent*> atributes);
  void accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~CreateSent();
};


// Insert Sentence de la gramática:
// insert_sent ::= INSERT INTO <id> VALUES '(' <value_exp> [',' <value_exp>]* ')'
class InsertSent: public Sentence {
public:
  string table;
  list<ValueExp*> values;
  InsertSent(string table, list<ValueExp*> values);
  void accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~InsertSent();
};


// Una table puede ser solo un alias como id o un alias con un query asociado
// [ '(' <query> ')' AS ] <id>
class SelectQuery;
class TableSent : public Sentence {
public:
  SelectQuery* query = nullptr;
  string alias;
  TableSent(SelectQuery* query, string alias);
  void accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~TableSent();
};


// Select Sentence de la forma:
// SELECT <id> [',' <id>]* FROM <table> (',' <table>)*
class SelectSent: public Sentence {
public:
  list<string> ids;
  list<TableSent*> tables;
  SelectSent(list<string> ids, list<TableSent*> tables);
  void accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~SelectSent();
};


// Where Sentence de la forma:
// WHERE <exp>
class WhereSent: public Sentence {
public:
  Exp* exp;
  WhereSent(Exp* exp);
  void accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~WhereSent();
};



// Una query es una una consulta SQL
class Query {
public:
  virtual void accept(ImpVisitor* v) = 0;
  virtual void accept(TypeVisitor* v) = 0;
  virtual ~Query() = 0;
};

#endif