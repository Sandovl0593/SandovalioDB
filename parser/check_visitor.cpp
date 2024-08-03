#include "check_visitor.hh"
#include <regex>

CheckVisitor::CheckVisitor() {
  env.add_level();
}

// ---------------------------------------------------------------- //
// subclases Value
ValueType CheckVisitor::visit(IntValue* v) {
  return TINT;
}

ValueType CheckVisitor::visit(StringValue* v) {
  // utilizando una expresion regular para verificar que sea un date
  if (regex_match(v->value, regex("^[0-9]{4}-[0-9]{2}-[0-9]{2}$")))
    return TDATE;
  return TVARCHAR;
}

ValueType CheckVisitor::visit(BoolValue* v) {
  return TBOOLEAN;
}

ValueType CheckVisitor::visit(FloatValue* v) {
  return TFLOAT;
}


// ---------------------------------------------------------------- //
// subclases de Exp
void CheckVisitor::visit(BinaryExp* e) {
  e->left->accept(this);
  e->right->accept(this);
}

void CheckVisitor::visit(UnaryExp* e) {
  e->e->accept(this);
}

void CheckVisitor::visit(ParenthExp* e) {
  e->e->accept(this);
}

void CheckVisitor::visit(FilterExp* e) {
  if (!env.check(e->id)) {
    cout << "Error: atributo no existente en filter";
    exit(0);
  }
  ValueType left = env.lookup(e->id);
  ValueType right = e->value->accept(this);
  if (left != right) {
    cout << "Error: tipos incompatibles en la expresion filter";
    exit(0);
  }
}

void CheckVisitor::visit(JoinExp* e) {
  if (!env.check(e->id1) || !env.check(e->id2)) {
    cout << "Error: atributo no existente en join";
    exit(0);
  }
  ValueType left = env.lookup(e->id1);
  ValueType right = env.lookup(e->id2);
  if (left != right) {
    cout << "Error: tipos incompatibles en la expresion join";
    exit(0);
  }
}

void CheckVisitor::visit(BetweenExp* e) {
  if (!env.check(e->id)) {
    cout << "Error: atributo no existente en between";
    exit(0);
  }
  ValueType left = env.lookup(e->id);
  ValueType right1 = e->left->accept(this);
  ValueType right2 = e->right->accept(this);
  if (left != right1 || left != right2) {
    cout << "Error: tipos incompatibles en la expresion between";
    exit(0);
  }
}

void CheckVisitor::visit(LikeExp* e) {
  if (!env.check(e->id)) {
    cout << "Error: atributo no existente en like";
    exit(0);
  }
  ValueType left = env.lookup(e->id);
  if (left != TVARCHAR && left != TDATE) {
    cout << "Error: tipos incompatibles en la expresion like";
    exit(0);
  }
}

void CheckVisitor::visit(InValueExp* e) {
  if (!env.check(e->id)) {
    cout << "Error: atributo no existente en inValue";
    exit(0);
  }
  ValueType left = env.lookup(e->id);
  for (auto v : e->values) {
    ValueType right = v->accept(this);
    if (left != right) {
      cout << "Error: tipos incompatibles en la expresion inValue";
      exit(0);
    }
  }
}

void CheckVisitor::visit(InQueryExp* e) {
  if (!env.check(e->id)) {
    cout << "Error: atributo no existente en inQuery";
    exit(0);
  }
  ValueType left = env.lookup(e->id);

  e->query->accept(this);
  // aqui ya se valida que los atributos sean correctos

  // verificando si sea SOLO un atributo
  if (env.get_current_level().size() != 1) {
    cout << "Error: la subconsulta de in solo puede devolver un atributo";
    exit(0);
  }
  // obteniendo el tipo del atributo
  ValueType right = env.get_current_level().begin()->second;

  if (left != right) {
    cout << "Error: tipos incompatibles en la expresion de inQuery";
    exit(0);
  }
}


// ---------------------------------------------------------------- //
// TableDec
void CheckVisitor::visit(TableDec* t) {
  if (t->query != nullptr) {
    t->query->accept(this);
    // hasta aqui la subconsulta ya esta validada
    for (auto a : t->query->select->ids) {
      if (!env.check(a)) {
        cout << "Error: atributo no existente en la subconsulta";
        exit(0);
      }
      env.add_var(a, env.lookup(a));
    }
  }
  else {
    if (!env.check(t->name_tableDec)) {
      cout << "Error: tabla no existente en from";
      exit(0);
    }
  }
}


// ---------------------------------------------------------------- //
// subclases de Sent
void CheckVisitor::visit(AtributeSent* s) {
  env.add_var(s->id + "$$" + s->table, s->type);
}

void CheckVisitor::visit(SelectSent* s) {
  for (auto id : s->ids) {
    if (!env.check(id)) {
      cout << "Error: atributo no existente en la sentencia select";
      exit(0);
    }
  }
}

void CheckVisitor::visit(WhereSent* s) {
  s->e->accept(this);
}

void CheckVisitor::visit(FromSent* s) {
  list<TableDec*>::iterator it1;
  list<JoinSent*>::iterator it2;
  for (it1 = s->tables.begin(), it2 = s->joins.begin(); it1 != s->tables.end(); it1++, it2++) {
    (*it1)->accept(this);
    if (*it2 != nullptr) (*it2)->accept(this);
  }
}

void CheckVisitor::visit(UpdateSent* s) {
  if (!env.check(s->table)) {
    cout << "Error: tabla no existente en update";
    exit(0);
  }
  list<string>::iterator it1;
  list<Value*>::iterator it2;
  for (it1 = s->ids.begin(), it2 = s->values.begin(); it1 != s->ids.end(); it1++, it2++) {
    if (!env.check(*it1)) {
      cout << "Error: atributo no existente en update";
      exit(0);
    }
    ValueType left = env.lookup(*it1);
    ValueType right = (*it2)->accept(this);
    if (left != right) {
      cout << "Error: tipos incompatibles en la sentencia update";
      exit(0);
    }
  }
}

void CheckVisitor::visit(LimitSent* s) {
  // no se necesita hacer nada
}

void CheckVisitor::visit(JoinSent* s) {
  s->table->accept(this);
  s->e->accept(this);
}

// ---------------------------------------------------------------- //
// subclases de Query y QueryList
void CheckVisitor::typecheck(QueryList* q) {
  q->accept(this);
}

void CheckVisitor::visit(QueryList* q) {
  for (auto query : q->queries) {
    query->accept(this);
  }
}

void CheckVisitor::visit(CreateQuery* q) {
  for (auto a : q->atributes) {
    a->accept(this);  // AtributeSent visitor
  }
}

void CheckVisitor::visit(InsertQuery* q) {
  if (!env.check(q->table)) {
    cout << "Error: tabla no existente en insert";
    exit(0);
  }
  for (auto v : q->values) {
    v->accept(this);
  }
}

void CheckVisitor::visit(SelectQuery* q) {
  q->from->accept(this);
  if (q->where != nullptr) q->where->accept(this);
  q->select->accept(this);
}