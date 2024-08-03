#ifndef CHECKER_HH
#define CHECKER_HH

#include <unordered_map>
#include "environment.hh"


// este visitor revisa el AST para verificar que sea correcto
class CheckVisitor {
public:
  CheckVisitor();
private:
  Environment<ValueType> env; // environment de atributos
public:
  void typecheck(QueryList*); 
  void visit(QueryList*);
  void visit(CreateQuery*);
  void visit(InsertQuery*);
  void visit(SelectQuery*);
  void visit(DeleteQuery*);
  void visit(UpdateQuery*);

  void visit(TableDec*);
  ValueType visit(IntValue*);
  ValueType visit(StringValue*);
  ValueType visit(BoolValue*);
  ValueType visit(FloatValue*);

  void visit(BinaryExp* e);
  void visit(UnaryExp* e);
  void visit(ParenthExp* e);
  void visit(FilterExp*);
  void visit(JoinExp*);
  void visit(BetweenExp*);
  void visit(LikeExp*);
  void visit(InValueExp*);
  void visit(InQueryExp*);

  void visit(AtributeSent*);
  void visit(SelectSent*);
  void visit(WhereSent*);
  void visit(FromSent*);
  void visit(UpdateSent*);
  void visit(LimitSent*);
  void visit(JoinSent*);
};


#endif