#include "print_visitor.hh"

const char* type_names[6] = { "notype", "varchar", "int", "boolean", "date", "float" };

// ---------------------------------------------------------------- //
// subclases de TableDec y Value

// imprime una declaracion de tabla
void PrintVisitor::visit(TableDec* t) {
  if (t->query != nullptr) {
    cout << "(" << endl;
    t->query->accept(this);
    cout << "\n) AS ";
  }
  cout << t->name_tableDec << " ";
}

// imprime un entero
void PrintVisitor::visit(IntValue* v) {
  cout << v->value;
}

// imprime una cadena
void PrintVisitor::visit(StringValue* v) {
  cout << "'" << v->value << "'";
}

// imprime un booleano
void PrintVisitor::visit(BoolValue* v) {
  cout << (v->value ? "true" : "false");
}

// imprime un flotante
void PrintVisitor::visit(FloatValue* v) {
  cout << v->value;
}



// ---------------------------------------------------------------- //
// subclases de Query y QueryList

// ejecuta el visitor sobre una lista de queries
void PrintVisitor::visit(QueryList* q) {
  for (auto query : q->queries) {
    query->accept(this);
    cout << ";\n" << endl;
  }
}

// imprime una query de creacion
void PrintVisitor::visit(CreateQuery* q) {
  cout << "CREATE TABLE " << q->table << " (" << endl;
  for (auto a : q->atributes) {
    cout << "  ";
    a->accept(this);
    cout << "," << endl;
  }
  cout << ")";
}

// imprime una query de insercion
void PrintVisitor::visit(InsertQuery* q) {
  cout << "INSERT INTO " << q->table << " VALUES (";
  for (auto v : q->values) {
    v->accept(this);
    cout << ", ";
  }
  cout << ")";
}

// imprime una query de seleccion
void PrintVisitor::visit(SelectQuery* q) {
  q->select->accept(this);
  q->from->accept(this);
  if (q->where != nullptr)
    q->where->accept(this);
  if (q->limit != nullptr)
    q->limit->accept(this);
}

// imprime una query de eliminacion
void PrintVisitor::visit(DeleteQuery* q) {
  cout << "DELETE FROM " << q->table;
  if (q->exp != nullptr)
    q->exp->accept(this);
}

// imprime una query de actualizacion
void PrintVisitor::visit(UpdateQuery* q) {
  q->update->accept(this);
  if (q->where != nullptr)
    q->where->accept(this);
}



// ---------------------------------------------------------------- //
// subclases de Exp

// imprime una expresion binaria
void PrintVisitor::visit(BinaryExp* e) {
  e->left->accept(this);
  cout << " " << e->op << " ";
  e->right->accept(this);
}

// imprime una expresion unaria
void PrintVisitor::visit(UnaryExp* e) {
  cout << UnaryExp::unopToString(e->op) << " ";
  e->e->accept(this);
}

// imprime una expresion entre parentesis
void PrintVisitor::visit(ParenthExp* e) {
  cout << "(";
  e->e->accept(this);
  cout << ")";
}

// imprime una expresion de filtro
void PrintVisitor::visit(FilterExp* e) {
  cout << e->id << " " << FilterExp::filopToString(e->op) << " ";
  e->value->accept(this);
}

// imprime una expresion de join
void PrintVisitor::visit(JoinExp* e) {
  cout << e->id1 << " = " << e->id2;
}

// imprime una expresion de between
void PrintVisitor::visit(BetweenExp* e) {
  cout << e->id << " BETWEEN ";
  e->left->accept(this);
  cout << " AND ";
  e->right->accept(this);
}

// imprime una expresion de like
void PrintVisitor::visit(LikeExp* e) {
  cout << e->id << " LIKE " << e->pattern;
}

// imprime una expresion de in con valores
void PrintVisitor::visit(InValueExp* e) {
  int size = e->values.size();
  cout << e->id << " IN (";
  for (auto it = e->values.begin(); it != e->values.end(); it++) {
    (*it)->accept(this);
    if (--size > 0) cout << ", ";
  }
  cout << ")";
}

// imprime una expresion de in con query
void PrintVisitor::visit(InQueryExp* e) {
  cout << e->id << " IN (" << endl;
  e->query->accept(this);
  cout << endl << ")";
}



// ---------------------------------------------------------------- //
// subclases de Sent

// imprime una sentencia de atributo
void PrintVisitor::visit(AtributeSent* s) {
  cout << s->id << " " << type_names[s->type];
  if (s->not_null) cout << " NOT NULL";
}

// imprime una sentencia de seleccion
void PrintVisitor::visit(SelectSent* s) {
  int size = s->ids.size();
  cout << "SELECT ";
  for (auto it = s->ids.begin(); it != s->ids.end(); it++) {
    cout << *it;
    if (--size > 0) cout << ", ";
  }
}

// imprime una sentencia de seleccion de tablas
void PrintVisitor::visit(FromSent* s) {
  list<TableDec*>::iterator it_atr;
  list<JoinSent*>::iterator it_join;
  int size = s->tables.size();
  cout << "\nFROM ";
  for (it_atr = s->tables.begin(), it_join = s->joins.begin(); it_atr != s->tables.end(); it_atr++, it_join++) {
    (*it_atr)->accept(this);
    if (*it_join != nullptr)
      (*it_join)->accept(this);

    if (--size > 0) cout << ", ";
  }
}

// imprime una sentencia de seleccion de condiciones
void PrintVisitor::visit(WhereSent* s) {
  cout << "\nWHERE ";
  s->e->accept(this);
}

// imprime una sentencia de actualizacion
void PrintVisitor::visit(UpdateSent* s) {
  list<string>::iterator it_id;
  list<Value*>::iterator it_val;
  int size = s->values.size();
  cout << "UPDATE " << s->table << "\nSET ";
  for (it_id = s->ids.begin(), it_val = s->values.begin(); it_id != s->ids.end(); it_id++, it_val++) {
    cout << *it_id << " = ";
    (*it_val)->accept(this);
    if (--size > 0) cout << ",\n    ";
  }
}

// imprime una sentencia de limite
void PrintVisitor::visit(LimitSent* s) {
  cout << "\nLIMIT " << s->limit;
}

// imprime una sentencia de join
void PrintVisitor::visit(JoinSent* s) {
  cout << " " << JoinSent::joinopToString(s->op) << " ";
  s->table->accept(this);
  cout << " ON ";
  s->e->accept(this);
}