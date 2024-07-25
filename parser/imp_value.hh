#ifndef IMP_VALUE_HH
#define IMP_VALUE_HH

#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>

using namespace std;

enum ValueType { NOTYPE=0, TVARCHAR, TINT, TBOOLEAN, TDATE }; // value type
const char* type_names[5] = { "notype", "varchar", "int", "boolean", "date" };


// Guarda el valor de acuerdo al tipo ValueType
class ValueData {
public:
  ValueData();
  ValueType type;
  int int_value;
  bool bool_value;
  string varchar_value;
  string date_value;
  bool set_basic_type(ValueType tt);
};


ValueData::ValueData():type(NOTYPE) { }

ostream& operator<<(ostream& outs, const ValueData& v) {
  if (v.type == TINT)
    outs << v.int_value;
  else if (v.type == TBOOLEAN)
    outs << (v.bool_value?"true":"false");
  else if (v.type == TVARCHAR)
    outs << v.varchar_value;
  else if (v.type == TDATE)
    outs << v.date_value;
  else
    outs << "NOTYPE";
  return outs;
}

bool ValueData::set_basic_type(ValueType tt) {
  bool ret = true;
  switch(tt) {
    case TDATE:
    case TVARCHAR:
    case TINT:
    case TBOOLEAN:
      this->type = tt; break;
    default:
      ret = false;
  }
  return ret;
}

std::ostream& operator<<( std::ostream& outs, const ValueType& type) {
  outs << type_names[type];
  return outs;
}

#endif