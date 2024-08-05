#ifndef IMP_VISITOR_HH
#define IMP_VISITOR_HH

#include "ast.hh"

class ImpVisitor {
public:
  virtual void visit(QueryList*) = 0;
  virtual void visit(CreateQuery*) = 0;
  virtual void visit(InsertQuery*) = 0;
  virtual void visit(SelectQuery*) = 0;
  virtual void visit(DeleteQuery*) = 0;
  virtual void visit(UpdateQuery*) = 0;

  virtual void visit(TableDec*) = 0;
  virtual void visit(IntValue*) = 0;
  virtual void visit(StringValue*) = 0;
  virtual void visit(BoolValue*) = 0;
  virtual void visit(FloatValue*) = 0;

  virtual void visit(BinaryExp* e) = 0;
  virtual void visit(UnaryExp* e) = 0;
  virtual void visit(ParenthExp* e) = 0;
  virtual void visit(FilterExp*) = 0;
  virtual void visit(JoinExp*) = 0;
  virtual void visit(BetweenExp*) = 0;
  virtual void visit(LikeExp*) = 0;
  virtual void visit(InValueExp*) = 0;
  virtual void visit(InQueryExp*) = 0;

  virtual void visit(AtributeSent*) = 0;
  virtual void visit(SelectSent*) = 0;
  virtual void visit(WhereSent*) = 0;
  virtual void visit(UpdateSent*) = 0;
  virtual void visit(LimitSent*) = 0;
  virtual void visit(JoinSent*) = 0;
};

#endif