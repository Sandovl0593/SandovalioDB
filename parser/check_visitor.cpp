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
  ValueType left = env.lookup(e->id);
  ValueType right = e->value->accept(this);
  if (left != right) {
    cout << "Error: tipos incompatibles en la expresion de filtro";
    exit(0);
  }
}

void CheckVisitor::visit(JoinExp* e) {
  e->accept(this);
}

void CheckVisitor::visit(BetweenExp* e) {
  ValueType left = env.lookup(e->id);
  ValueType right1 = e->left->accept(this);
  ValueType right2 = e->right->accept(this);
  if (left != right1 || left != right2) {
    cout << "Error: tipos incompatibles en la expresion de between";
    exit(0);
  }
}

void CheckVisitor::visit(LikeExp* e) {
  ValueType left = env.lookup(e->id);
  if (left != TVARCHAR || left != TDATE) {
    cout << "Error: tipos incompatibles en la expresion de like";
    exit(0);
  }
}

void CheckVisitor::visit(InValueExp* e) {
  ValueType left = env.lookup(e->id);
  for (auto v : e->values) {
    ValueType right = v->accept(this);
    if (left != right) {
      cout << "Error: tipos incompatibles en la expresion de in";
      exit(0);
    }
  }
}

void CheckVisitor::visit(InQueryExp* e) {
  ValueType left = env.lookup(e->id);
  env.add_level();
  e->query->accept(this); // aqui ya se valida que los atributos sean correctos
  
  // verificando si sea SOLO un atributo
  if (env.get_current_level().size() != 1) {
    cout << "Error: la subconsulta de in solo puede devolver un atributo";
    exit(0);
  }
  // obteniendo el tipo del atributo
  ValueType right = env.get_current_level().begin()->second;
  env.remove_level();
  if (left != right) {
    cout << "Error: tipos incompatibles en la expresion de in";
    exit(0);
  }
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
    a->accept(this);
  }
}

void CheckVisitor::visit(InsertQuery* q) {
  for (auto v : q->values) {
    v->accept(this);
  }
}

void CheckVisitor::visit(SelectQuery* q) {
  q->from->accept(this);
  q->select->accept(this);
  if (q->where != nullptr) q->where->accept(this);
  if (q->limit != nullptr) q->limit->accept(this);
}