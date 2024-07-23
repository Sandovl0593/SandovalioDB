#ifndef IMP_VALUE_HH
#define IMP_VALUE_HH

#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>

using namespace std;

enum TType { NOTYPE=0, TVARCHAR, TINT, TBOOLEAN, TDATE }; // value type

class ImpValue {
public:
  ImpValue();
  TType type;
  int int_value;
  bool bool_value;
  string varchar_value;
  string date_value;
};


ImpValue::ImpValue():type(NOTYPE) { }

ostream& operator<<(ostream& outs, const ImpValue& v) {
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
#endif