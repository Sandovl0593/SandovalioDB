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

class SelectQuery;

// ---------------------------------------------------------------- //
// Toda expresion contiene un operador y uno o dos operandos
class Exp {
public:
  virtual void accept(ImpVisitor* v) = 0;
  virtual ImpType accept(TypeVisitor* v) = 0;
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
  static string binopToString(BinaryOp op);
  ~BinaryExp();
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


// Expresion de la forma: <op> <e>
class UnaryExp : public Exp {
public:
  UnaryOp op;
  Exp *e;
  UnaryExp(UnaryOp op, Exp *e);
  void accept(ImpVisitor* v);
  ImpType accept(TypeVisitor* v);
  static string unaryopToString(UnaryOp op);
  ~UnaryExp();
};


// Expresion de filtro de la forma: <id> <op> <value_exp>
class FilterExp : public Exp {
public:
  string id;
  ValueExp *value;
  FilterOp op;
  FilterExp(string id, ValueExp* value, FilterOp op);
  void accept(ImpVisitor* v);
  ImpType accept(TypeVisitor* v);
  static string filteropToString(FilterOp op);
  ~FilterExp();
};


// Expresion con Between de la forma: <id> BETWEEN <left> AND <right>
class BetweenExp : public Exp {
public:
  string id;
  ValueExp *left;
  ValueExp *right;
  BetweenExp(string id, ValueExp* l, ValueExp* r);
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


// Expresion de la forma: <id> '=' <id>
class JoinExp : public Exp {
public:
  string id1;
  string id2;
  JoinExp(string id1, string id2);
  void accept(ImpVisitor* v);
  ImpType accept(TypeVisitor* v);
  ~JoinExp();
};


// Expresion de la forma: <id> IN '(' <value_exp> [',' <value_exp>]* ')'
class InValueExp : public Exp {
public:
  string id;
  list<ValueExp*> values;
  InValueExp(list<ValueExp*> values);
  void accept(ImpVisitor* v);
  ImpType accept(TypeVisitor* v);
  ~InValueExp();
};


// Expresion de la forma: <id> IN '(' <select_query> ')'
class InQueryExp : public Exp {
public:
  string id;
  SelectQuery* query;
  InQueryExp(string id, SelectQuery* query);
  void accept(ImpVisitor* v);
  ImpType accept(TypeVisitor* v);
  ~InQueryExp();
};



// ---------------------------------------------------------------- //
// Un sentence es una ejecución interna dentro de uma consulta SQL
// Mas adelante, habrá un plan de ejecución donde en una query, las sentences se ejecutarán en un orden determinado
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


// Una table puede ser solo un name_table como id o un name_table con un query asociado
// ['(' <select_query> ')'] AS <name_table>
class TableSent: public Sentence {
public:
  string name_table;
  SelectQuery* query = nullptr;
  TableSent(string name_table);
  TableSent(string name_table, SelectQuery* query);
  void accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~TableSent();
};


// Select Sentence de la forma:
// SELECT <id> (',' <id>)*
class SelectSent: public Sentence {
public:
  list<string> ids;
  SelectSent(list<string> ids, list<TableSent*> tables);
  void accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~SelectSent();
};


// Referencia a tabla de la forma:
// FROM <table> [<join_sent>] (',' <table> [<join_sent>])*
class FromSent: public Sentence {
public:
  list<TableSent*> tables;
  list<JoinSent*> joins;   // si una tabla no tiene join, su join en la lista es nullptr
  FromSent(list<TableSent*> tables, list<JoinSent*> joins);
  void accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~FromSent();
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


// Update Sentence de la forma:
// UPDATE <table> SET <id> '=' <value_exp> [',' <id> '=' <value_exp>]*
class UpdateSent: public Sentence {
public:
  string table;
  list<string> ids;
  list<ValueExp*> values;
  UpdateSent(string table, list<string> ids, list<ValueExp*> values);
  void accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~UpdateSent();
};


// Limit Sentence de la forma:
// LIMIT <int>
class LimitSent: public Sentence {
public:
  int limit;
  LimitSent(int limit);
  void accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~LimitSent();
};


// join Sentence de la forma:
// [INNER | LEFT | RIGHT | CROSS] JOIN <table> ON <join_exp>
class JoinSent: public Sentence {
public:
  TableSent* table;
  JoinExp* join;
  JoinSent(TableSent* table);
  JoinSent(TableSent* table, JoinExp* join);
  void accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~JoinSent();
};



// ---------------------------------------------------------------- //
// Una query es una consulta SQL
class Query {
public:
  virtual void accept(ImpVisitor* v) = 0;
  virtual void accept(TypeVisitor* v) = 0;
  virtual ~Query() = 0;
};


// Create Query de la forma:
// CREATE TABLE <table> '(' <atribute> [',' <atribute>]* ')'
class CreateQuery: public Query {
public:
  string table;
  list<AtributeSent*> atributes;
  CreateQuery(string table, list<AtributeSent*> atributes);
  void accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~CreateQuery();
};


// Select Query de la forma:
// <select_sent> <from_sent> [ <where_sent> ] [ <limit_sent> ]
class SelectQuery: public Query {
public:
  SelectSent* select = nullptr; // es nullptr si se seleccionan todas las columnas
  FromSent* from;
  WhereSent* where = nullptr;
  LimitSent* limit = nullptr;
  SelectQuery(FromSent* from, SelectSent* select, WhereSent* where, LimitSent* limit);
  void accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~SelectQuery();
};


// Insert Query de la forma:
// INSERT INTO <id> VALUES '(' <value_exp> [',' <value_exp>]* ')'
class InsertQuery: public Query {
public:
  string table;
  list<ValueExp*> values;
  InsertQuery(string table, list<ValueExp*> values);
  void accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~InsertQuery();
};


// Update Query de la forma:
// <update_sent> [ <where_sent> ] 
class UpdateQuery: public Query {
public:
  UpdateSent* update;
  WhereSent* where = nullptr;
  UpdateQuery(UpdateSent* update, WhereSent* where);
  void accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~UpdateQuery();
};


// Delete Query de la forma:
// DELETE FROM <table> WHERE <exp>
class DeleteQuery: public Query {
public:
  string table;
  Exp* exp = nullptr;
  DeleteQuery(string table, Exp* exp);
  void accept(ImpVisitor* v);
  void accept(TypeVisitor* v);
  ~DeleteQuery();
};

#endif