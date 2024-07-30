#ifndef PLAN_EXEC_VISITOR_HH
#define PLAN_EXEC_VISITOR_HH

#include "ast.hh"
#include "imp_visitor.hh"
#include "check_visitor.hh"

// este visitor construye el plan de ejecucion SQL a partir del AST
class PlanExecVisitor : public ImpVisitor {
private:
  CheckVisitor* checker;
public:
  PlanExecVisitor(CheckVisitor* checker);
  void buildPlan(QueryList* q);

  void visit(QueryList* q);
  void visit(CreateQuery* q);
  void visit(InsertQuery* q);
  void visit(SelectQuery* q);
  void visit(DeleteQuery* q);
  void visit(UpdateQuery* q);

  void visit(TableDec* t);
  void visit(IntValue* v);
  void visit(StringValue* v);
  void visit(BoolValue* v);
  void visit(FloatValue* v);

  void visit(BinaryExp* e);
  void visit(UnaryExp* e);
  void visit(ParenthExp* e);
  void visit(FilterExp* e);
  void visit(JoinExp* e);
  void visit(BetweenExp* e);
  void visit(LikeExp* e);
  void visit(InValueExp* e);
  void visit(InQueryExp* e);

  void visit(AtributeSent* s);
  void visit(SelectSent* s);
  void visit(WhereSent* s);
  void visit(FromSent* s);
  void visit(UpdateSent* s);
  void visit(LimitSent* s);
  void visit(JoinSent* s);
};

#endif