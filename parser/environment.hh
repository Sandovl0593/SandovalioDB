#ifndef ENV
#define ENV

#include <unordered_map>
#include <string>
#include <fstream>
#include "ast.hh"
using namespace std;

// en el archivo env_tables.dat
// cada linea es de tipo: <nombre_tabla> <nombre_atributo> <tipo> or
//                        subq_<nombre_tabla> <nombre_atributo> <tipo>
class Environment {
  fstream file;
public:
  Environment() {
    // read from file
    file.open("env_tables.dat", ios::in);
    file.close();
  }
  void add_var(string table, string atr, ValueType value) {
    file.open("env_tables.dat", ios::app);
    file << table << " " << atr << " " << word_type(value) << endl;
    file.close();
  }
  
  void remove_subqueries() {
    fstream temp;
    temp.open("temp.dat", ios::out);
    file.open("env_tables.dat", ios::in);
    string table, atr, type;
    while (file >> table >> atr >> type) {
      if (table.substr(0, 4) != "subq") {
        temp << table << " " << atr << " " << type << endl;
      }
    }
    file.close();
    temp.close();
    remove("env_tables.dat");
    rename("temp.dat", "env_tables.dat");
  }

  ValueType check_word(string word) {
    if (word == "int") return TINT;
    if (word == "varchar") return TVARCHAR;
    if (word == "boolean") return TBOOLEAN;
    if (word == "float") return TFLOAT;
    else  return TDATE;
  }

  string word_type(ValueType type) {
    if (type == TINT) return "int";
    if (type == TVARCHAR) return "varchar";
    if (type == TBOOLEAN) return "boolean";
    if (type == TFLOAT) return "float";
    else return "date";
  }

  ValueType lookup(string table, string atr = "") {
    file.open("env_tables.dat", ios::in);
    string table2, atr2, type;
    while (file >> table2 >> atr2 >> type) {
      // si atr es vacio, solo se busca la tabla
      if (atr == "" && table == table2) {
        file.close();
        return check_word(type);
      }
      else if (table == table2 && atr == atr2) {
        file.close();
        return check_word(type);
      }
    }
    file.close();
    return NOTYPE;
  }
};

#endif