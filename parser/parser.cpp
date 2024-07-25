#include "parser.hh"

void Parser::setInput(string value) {
  scanner->setInput(value);
}

void Parser::throwParser(string err) {
  if (report.empty()) report = err;
  // clearMemory();
}

string Parser::reportParse() { return reportFin; }

void Parser::resetParser() {
  scanner->resetScanner();
  if (current)  delete current;
  if (previous) delete previous;
  current = previous = NULL;
  reportFin = report;
  report = "";
}

// match and consume next token
bool Parser::match(TokenType ttype) {
  if (check(ttype)) {
    advance();
    return true;
  }
  return false;
}

bool Parser::check(TokenType ttype) {
  //if (isAtEnd()) return false;
  return current->type == ttype;
}

void Parser::advance() {
  if (!isAtEnd()) {
    Token* temp =current;
    if (previous) delete previous;
    current = scanner->nextToken();
    previous = temp;
    if (check(ERR)) {
      cout << "Parse error, unrecognised character: " << current->lexema << endl;
      exit(0);
    }
  }
}

bool Parser::isAtEnd() {
  return (current->type == END);
}
