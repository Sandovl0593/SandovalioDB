#include "ast.hh"
#include "imp_visitor.hh"
#include "check_visitor.hh"

string BinaryExp::binopToString(BinaryOp op) {
  return (op == AND) ? "AND" : "OR";
}

string UnaryExp::unopToString(UnaryOp op) {
  return (op == NOT) ? "NOT " : "-";
}

string FilterExp::filopToString(FilterOp op) {
  switch(op) {
    case EQ: return "==";
    case NEQ: return "<>";
    case LT: return "<";
    case LTEQ: return "<=";
    case GT: return ">";
    case GTEQ: return ">=";
  }
  return "";
}

string JoinSent::joinopToString(JoinOp op) {
  switch (op) {
    case INNER: return "INNER JOIN";
    case LEFT: return "LEFT JOIN";
    case RIGHT: return "RIGHT JOIN";
    case CROSS: return "CROSS JOIN";
  }
  return "";
}

// ---------------------------------------------------------------- //
// Cons. y Des. clase TableDec
TableDec::TableDec(string name_table): name_tableDec(name_table) {}
TableDec::TableDec(string name_table, SelectQuery* query): name_tableDec(name_table), query(query) {}
TableDec::~TableDec() {
  if (query != nullptr) delete query;
}

// ---------------------------------------------------------------- //
// Cons. y Des. de la clase Value
IntValue::IntValue(int value): value(value) {}
IntValue::~IntValue() {}

StringValue::StringValue(string value): value(value) {}
StringValue::~StringValue() {}

BoolValue::BoolValue(bool value): value(value) {}
BoolValue::~BoolValue() {}

FloatValue::FloatValue(float value): value(value) {}
FloatValue::~FloatValue() {}

// ---------------------------------------------------------------- //
// Cons. y Desc. de las clases Exp
BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op): left(l), right(r), op(op) {}
BinaryExp::~BinaryExp() { delete left; delete right; }

UnaryExp::UnaryExp(Exp* e, UnaryOp op): e(e), op(op) {}
UnaryExp::~UnaryExp() { delete e; }

ParenthExp::ParenthExp(Exp* e): e(e) {}
ParenthExp::~ParenthExp() { delete e; }

FilterExp::FilterExp(string tref, string id, Value* value, FilterOp op): tref(tref), id(id), value(value), op(op) {}
FilterExp::~FilterExp() { delete value; }

BetweenExp::BetweenExp(string tref, string id, Value* l, Value* r): tref(tref), id(id), left(l), right(r) {}
BetweenExp::~BetweenExp() { delete left; delete right; }

LikeExp::LikeExp(string tref, string id, string pattern): tref(tref), id(id), pattern(pattern) {}
LikeExp::~LikeExp() {}

JoinExp::JoinExp(string tref1, string tref2, string id1, string id2): tref1(tref1), tref2(tref2), id1(id1), id2(id2) {}
JoinExp::~JoinExp() {}

InValueExp::InValueExp(string tref, string id, list<Value*> values): tref(tref), id(id), values(values) {}
InValueExp::~InValueExp() {
  for (auto v : values) delete v;
}

InQueryExp::InQueryExp(string tref, string id, SelectQuery* query): tref(tref), id(id), query(query) {}
InQueryExp::~InQueryExp() { delete query; }


// ---------------------------------------------------------------- //
// Cons. y Desc. de las clases Sentence
AtributeSent::AtributeSent(string table, string id, ValueType type, bool not_null) : table(table), id(id), type(type), not_null(not_null) {}
AtributeSent::~AtributeSent() {}

SelectSent::SelectSent(list<string> ids, list<TableDec*> tables, list<JoinSent*> joins): ids(ids), tables(tables), joins(joins) {}
SelectSent::~SelectSent() {
  for (auto t : tables) delete t;
  for (auto j : joins) delete j;
}

WhereSent::WhereSent(Exp* e): e(e) {}
WhereSent::~WhereSent() { delete e; }

UpdateSent::UpdateSent(string table, list<string> ids, list<Value*> values): table(table), ids(ids), values(values) {}
UpdateSent::~UpdateSent() {
  for (auto v : values) delete v;
}

LimitSent::LimitSent(int limit): limit(limit) {}
LimitSent::~LimitSent() {}

JoinSent::JoinSent(JoinOp op, TableDec* table, JoinExp* e): op(op), table(table), e(e) {}
JoinSent::~JoinSent() { delete table; delete e; }


// ---------------------------------------------------------------- //
// Cons. y Desc. de las clases Query
QueryList::QueryList(list<Query*> queries): queries(queries) {}
QueryList::~QueryList() {
  for (auto q : queries) delete q;
}

CreateQuery::CreateQuery(string table, list<AtributeSent*> atributes): table(table), atributes(atributes) {}
CreateQuery::~CreateQuery() {
  for (auto a : atributes) delete a;
}

SelectQuery::SelectQuery(SelectSent* select, WhereSent* where, LimitSent* limit): select(select), where(where), limit(limit) {}
SelectQuery::~SelectQuery() { delete select; delete where; delete limit; }

InsertQuery::InsertQuery(string table, list<Value*> values): table(table), values(values) {}
InsertQuery::~InsertQuery() {
  for (auto v : values) delete v;
}

UpdateQuery::UpdateQuery(UpdateSent* update, WhereSent* where) : update(update), where(where) {}
UpdateQuery::~UpdateQuery() { delete update; delete where; }

DeleteQuery::DeleteQuery(string table, Exp* exp) : table(table), exp(exp) {}
DeleteQuery::~DeleteQuery() { delete exp; }


// ---------------------------------------------------------------- //
// ImpVisitors
void QueryList::accept(ImpVisitor* v) { v->visit(this); }
void CreateQuery::accept(ImpVisitor* v) { v->visit(this); }
void InsertQuery::accept(ImpVisitor* v) { v->visit(this); }
void SelectQuery::accept(ImpVisitor* v) { v->visit(this); }
void DeleteQuery::accept(ImpVisitor* v) { v->visit(this); }
void UpdateQuery::accept(ImpVisitor* v) { v->visit(this); }

void TableDec::accept(ImpVisitor* v) { v->visit(this); }
void IntValue::accept(ImpVisitor* v) { v->visit(this); }
void StringValue::accept(ImpVisitor* v) { v->visit(this); }
void BoolValue::accept(ImpVisitor* v) { v->visit(this); }
void FloatValue::accept(ImpVisitor* v) { v->visit(this); }

void BinaryExp::accept(ImpVisitor* v) { v->visit(this); }
void UnaryExp::accept(ImpVisitor* v) { v->visit(this); }
void ParenthExp::accept(ImpVisitor* v) { v->visit(this); }
void FilterExp::accept(ImpVisitor* v) { v->visit(this); }
void JoinExp::accept(ImpVisitor* v) { v->visit(this); }
void BetweenExp::accept(ImpVisitor* v) { v->visit(this); }
void LikeExp::accept(ImpVisitor* v) { v->visit(this); }
void InValueExp::accept(ImpVisitor* v) { v->visit(this); }
void InQueryExp::accept(ImpVisitor* v) { v->visit(this); }

void AtributeSent::accept(ImpVisitor* v) { v->visit(this); }
void SelectSent::accept(ImpVisitor* v) { v->visit(this); }
void WhereSent::accept(ImpVisitor* v) { v->visit(this); }
void UpdateSent::accept(ImpVisitor* v) { v->visit(this); }
void LimitSent::accept(ImpVisitor* v) { v->visit(this); }
void JoinSent::accept(ImpVisitor* v) { v->visit(this); }


// ---------------------------------------------------------------- //
// CheckVisitors
void QueryList::accept(CheckVisitor* v) { v->visit(this); }
void CreateQuery::accept(CheckVisitor* v) { v->visit(this); }
void InsertQuery::accept(CheckVisitor* v) { v->visit(this); }
void SelectQuery::accept(CheckVisitor* v) { v->visit(this); }
void DeleteQuery::accept(CheckVisitor* v) { v->visit(this); }
void UpdateQuery::accept(CheckVisitor* v) { v->visit(this); }

void TableDec::accept(CheckVisitor* v) { v->visit(this); }
ValueType IntValue::accept(CheckVisitor* v) { return v->visit(this); }
ValueType StringValue::accept(CheckVisitor* v) { return v->visit(this); }
ValueType BoolValue::accept(CheckVisitor* v) { return v->visit(this); }
ValueType FloatValue::accept(CheckVisitor* v) { return v->visit(this); }

void BinaryExp::accept(CheckVisitor* v) { v->visit(this); }
void UnaryExp::accept(CheckVisitor* v) { v->visit(this); }
void ParenthExp::accept(CheckVisitor* v) { v->visit(this); }
void FilterExp::accept(CheckVisitor* v) { v->visit(this); }
void JoinExp::accept(CheckVisitor* v) { v->visit(this); }
void BetweenExp::accept(CheckVisitor* v) { v->visit(this); }
void LikeExp::accept(CheckVisitor* v) { v->visit(this); }
void InValueExp::accept(CheckVisitor* v) { v->visit(this); }
void InQueryExp::accept(CheckVisitor* v) { v->visit(this); }

void AtributeSent::accept(CheckVisitor* v) { v->visit(this); }
void SelectSent::accept(CheckVisitor* v) { v->visit(this); }
void WhereSent::accept(CheckVisitor* v) { v->visit(this); }
void UpdateSent::accept(CheckVisitor* v) { v->visit(this); }
void LimitSent::accept(CheckVisitor* v) { v->visit(this); }
void JoinSent::accept(CheckVisitor* v) { v->visit(this); }
