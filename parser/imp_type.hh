#ifndef IMP_TYPE_HH
#define IMP_TYPE_HH

#include "imp_value.hh"
using namespace std;

const char* type_names[5] = { "notype", "varchar", "int", "boolean", "date" };

class ImpType {
public:
  TType ttype;
  bool set_basic_type(string s);
  bool set_basic_type(TType tt);
private:
  TType string_to_type(string s);
};


// valida si el tipo de dato es valido a partir de un string
bool ImpType::set_basic_type(string s) {
  TType tt;
  bool ret = true;
  tt = string_to_type(s);
  if (tt == NOTYPE) ret = false;
  this->ttype = tt;
  return ret;
}

// valida si el tipo de dato es valido a partir de un tipo de dato
bool ImpType::set_basic_type(TType tt) {
  bool ret = true;
  switch(tt) {
    case TDATE:
    case TVARCHAR:
    case TINT:
    case TBOOLEAN:
      this->ttype = tt; break;
    default:
      ret = false;
  }
  return ret;
}

// convierte un string a un tipo de dato SQL
TType ImpType::string_to_type(string s) {
  TType tt;
  if (s.compare("int")==0 || s.compare("INT")==0) tt = TINT;
  else if (s.compare("varchar")==0 || s.compare("VARCHAR")==0) tt = TVARCHAR;
  else if (s.compare("boolean")==0 || s.compare("BOOLEAN")==0) tt = TBOOLEAN;
  else if (s.compare("date")==0 || s.compare("DATE")==0) tt = TDATE;
  else tt = NOTYPE; 
  return tt;
}

std::ostream& operator<<( std::ostream& outs, const ImpType & type) {
  outs << type_names[type.ttype];
  return outs;
}

#endif