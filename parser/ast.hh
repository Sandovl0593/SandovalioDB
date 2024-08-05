#ifndef AST_HH
#define AST_HH

#include <iostream>
#include <cstring>
#include <list>

using namespace std;

enum FilterOp { EQ, NEQ, LT, GT, LTEQ, GTEQ };
enum JoinOp { INNER, LEFT, RIGHT, CROSS };
enum BinaryOp { AND, OR };
enum UnaryOp { NEG, NOT };
enum ValueType { NOTYPE=0, TVARCHAR, TINT, TBOOLEAN, TDATE, TFLOAT }; // value types

class ImpVisitor;
class CheckVisitor;
class SelectQuery;


// ---------------------------------------------------------------- //
// Una tabla es una referencia a una tabla en una base de datos 
// que puede ser solo un name_table o un name_table como alias de un query asociado como la forma:
// ['(' <select_query> ')' AS] <name_table>
class TableDec {
public:
  string name_tableDec;
  SelectQuery* query = nullptr;
  TableDec(string name_table);
  TableDec(string name_table, SelectQuery* query);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
  ~TableDec();
};



// ---------------------------------------------------------------- //
// Un value es un valor que puede ser utilizado en una expresion que puede ser <int>, <varchar>, <boolean>, <date>
class Value {
public:
  virtual void accept(ImpVisitor* v) = 0;
  virtual ValueType accept(CheckVisitor* v) = 0;
  virtual ~Value() = 0;
};


// Value de tipo int
class IntValue : public Value {
public:
  int value;
  IntValue(int value);
  void accept(ImpVisitor* v);
  ValueType accept(CheckVisitor* v);
  ~IntValue();
};


// Value de tipo string (en el checker se verifica si es un varchar o un date)
class StringValue : public Value {
public:
  string value;
  StringValue(string value);
  void accept(ImpVisitor* v);
  ValueType accept(CheckVisitor* v);
  ~StringValue();
};


// Value de tipo boolean
class BoolValue : public Value {
public:
  bool value;
  BoolValue(bool value);
  void accept(ImpVisitor* v);
  ValueType accept(CheckVisitor* v);
  ~BoolValue();
};


// Value de tipo float
class FloatValue : public Value {
public:
  float value;
  FloatValue(float value);
  void accept(ImpVisitor* v);
  ValueType accept(CheckVisitor* v);
  ~FloatValue();
};



// ---------------------------------------------------------------- //
// Toda expresion es una condicional con un operador y uno o dos operandos
class Exp {
public:
  virtual void accept(ImpVisitor* v) = 0;
  virtual void accept(CheckVisitor* v) = 0;
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
  void accept(CheckVisitor* v);
  static string binopToString(BinaryOp op);
  ~BinaryExp();
};


// Expresion de la forma: <op> <e>
class UnaryExp : public Exp {
public:
  UnaryOp op;
  Exp *e;
  UnaryExp(Exp *e, UnaryOp op);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
  static string unopToString(UnaryOp op);
  ~UnaryExp();
};


// Expresion de la forma: '(' <e> ')'
class ParenthExp : public Exp {
public:
  Exp *e;
  ParenthExp(Exp *e);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
  ~ParenthExp();
};


// Expresion de filtro de la forma: <id> <op> <value>
class FilterExp : public Exp {
public:
  string tref;
  string id;
  Value *value;
  FilterOp op;
  FilterExp(string tref, string id, Value* value, FilterOp op);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
  static string filopToString(FilterOp op);
  ~FilterExp();
};


// Expresion con Between de la forma: <id> BETWEEN <left> AND <right>
class BetweenExp : public Exp {
public:
  string tref;
  string id;
  Value *left;
  Value *right;
  BetweenExp(string tref, string id, Value* l, Value* r);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
  ~BetweenExp();
};


// Expresion con Like de la forma: <id> LIKE <pattern>
class LikeExp : public Exp {
public:
  string tref;
  string id;
  string pattern;
  LikeExp(string tref, string id, string pattern);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
  ~LikeExp();
};


// Expresion de la forma: <id> '=' <id>
class JoinExp : public Exp {
public:
  string tref1, tref2;
  string id1, id2;
  JoinExp(string tref1, string tref2, string id1, string id2);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
  ~JoinExp();
};


// Expresion de la forma: <id> IN '(' <value> [',' <value>]* ')'
class InValueExp : public Exp {
public:
  string tref;
  string id;
  list<Value*> values;
  InValueExp(string tref, string id, list<Value*> values);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
  ~InValueExp();
};


// Expresion de la forma: <id> IN '(' <select_query> ')'
class InQueryExp : public Exp {
public:
  string tref;
  string id;
  SelectQuery* query;
  InQueryExp(string tref, string id, SelectQuery* query);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
  ~InQueryExp();
};



// ---------------------------------------------------------------- //
// Un sentence es una ejecución interna dentro de uma consulta SQL
// Mas adelante, habrá un plan de ejecución donde en una query las sentences se ejecutarán en un orden determinado
class Sentence {
public:
  virtual void accept(ImpVisitor* v) = 0;
  virtual void accept(CheckVisitor* v) = 0;
  virtual ~Sentence() = 0;
};


// Atribute Sentence de la forma:
// <id> <type> [NOT NULL]
class AtributeSent: public Sentence {
public:
  string table;
  string id;
  ValueType type;
  bool not_null;
  AtributeSent(string table, string id, ValueType type, bool not_null);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
  ~AtributeSent();
};


// Select Sentence de la forma:
// SELECT <id> (',' <id>)*
// FROM <table> [<join_sent>] (',' <table> [<join_sent>])*
class SelectSent: public Sentence {
public:
  list<string> ids;
  list<TableDec*> tables;
  list<JoinSent*> joins;   // si una tabla no tiene join, su join en la lista es nullptr
  SelectSent(list<string> ids, list<TableDec*> tables, list<JoinSent*> joins);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
  ~SelectSent();
};


// Where Sentence de la forma:
// WHERE <exp>
class WhereSent: public Sentence {
public:
  Exp* e;
  WhereSent(Exp* e);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
  ~WhereSent();
};


// Update Sentence de la forma:
// UPDATE <table> SET <id> '=' <value> [',' <id> '=' <value>]*
class UpdateSent: public Sentence {
public:
  string table;
  list<string> ids;
  list<Value*> values;
  UpdateSent(string table, list<string> ids, list<Value*> values);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
  ~UpdateSent();
};


// Limit Sentence de la forma:
// LIMIT <int>
class LimitSent: public Sentence {
public:
  int limit;
  LimitSent(int limit);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
  ~LimitSent();
};


// join Sentence de la forma:
// [INNER | LEFT | RIGHT | CROSS] JOIN <table> ON <join_exp>
class JoinSent: public Sentence {
public:
  JoinOp op;
  TableDec* table;
  JoinExp* e;
  JoinSent(JoinOp op, TableDec* table, JoinExp* e);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
  static string joinopToString(JoinOp op);
  ~JoinSent();
};



// ---------------------------------------------------------------- //
// Una query es una consulta SQL
class Query {
public:
  virtual void accept(ImpVisitor* v) = 0;
  virtual void accept(CheckVisitor* v) = 0;
  virtual ~Query() = 0;
};


// Create Query de la forma:
// CREATE TABLE <table> '(' <atribute_sent> [',' <atribute_sent>]* ')'
class CreateQuery: public Query {
public:
  string table;
  list<AtributeSent*> atributes;
  CreateQuery(string table, list<AtributeSent*> atributes);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
  ~CreateQuery();
};


// Select Query de la forma:
// <select_sent> [ <where_sent> ] [ <limit_sent> ]
class SelectQuery: public Query {
public:
  SelectSent* select;
  WhereSent* where;
  LimitSent* limit;
  SelectQuery(SelectSent* select, WhereSent* where, LimitSent* limit);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
  ~SelectQuery();
};


// Insert Query de la forma:
// INSERT INTO <id> VALUES '(' <value> [',' <value>]* ')'
class InsertQuery: public Query {
public:
  string table;
  list<Value*> values;
  InsertQuery(string table, list<Value*> values);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
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
  void accept(CheckVisitor* v);
  ~UpdateQuery();
};


// Delete Query de la forma:
// DELETE FROM <table> WHERE <exp>
class DeleteQuery: public Query {
public:
  string table;
  Exp* exp;
  DeleteQuery(string table, Exp* exp);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
  ~DeleteQuery();
};



// ---------------------------------------------------------------- //
// Lista de Queries
class QueryList {
public:
  list<Query*> queries;
  QueryList(list<Query*> queries);
  void accept(ImpVisitor* v);
  void accept(CheckVisitor* v);
  ~QueryList();
};

#endif