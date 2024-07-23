#include "token.hh"

//------- Sign class
Sign::Sign(SignType t) : type(t) {}
Sign::Sign(SignType t, char err_c) : type(t), error(err_c) {}

ostream& operator<<(ostream& os, const Sign* sign) {
  if (sign->type == ERR)
    os << "ERR(" << sign->error << ")";
  else
    os << sign_names[sign->type];
  return os;
}


//------ Keyword class
Keyword::Keyword(KeywordType t) : type(t) {}

ostream& operator<<(ostream& os, const Keyword* keyword) {
  os << keyword_names[keyword->type];
  return os;
}


//------ Value class
Value::Value(ValueType t) : type(t) {}
Value::Value(ValueType t, const string source) : type(t), lexema(source) {}

ostream& operator<<(ostream& os, const Value* value) {
  os << value_names[value->type] << "(" << value->lexema << ")";
  return os;
}