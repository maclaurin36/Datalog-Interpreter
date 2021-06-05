//
// Created by Jesse Clark on 5/15/21.
//

#ifndef PROJECT_0_INTERPRETER_H
#define PROJECT_0_INTERPRETER_H
#include "DatalogProgram.h"
#include "Database.h"
#include "Graph.h"
#include <iostream>
#include <sstream>

class Interpreter {
private:
    DatalogProgram* program;
    Database* database;
public:
    Interpreter(DatalogProgram* program, Database* database);
    ~Interpreter();
    void Run();
    Relation* EvaluatePredicate(Predicate& p);
    std::string QueryResultToString(Predicate* query, Relation* relation);
};


#endif //PROJECT_0_INTERPRETER_H
