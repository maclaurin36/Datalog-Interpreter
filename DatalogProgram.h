//
// Created by Jesse Clark on 5/7/21.
//

#ifndef PROJECT_0_DATALOGPROGRAM_H
#define PROJECT_0_DATALOGPROGRAM_H

#include "Predicate.h"
#include "Rule.h"
#include <sstream>
#include <set>
// This class will be returned for Lab 3 to use
class DatalogProgram {
private:
    std::vector<Predicate*> schemes;
    std::vector<Predicate*> facts; //(This is also where the Domain is, print out in alphabetical order using a set for pass off cases)
    std::vector<Predicate*> queries;
    std::vector<Rule*> rules;
    std::set<std::string> domain;
public:
    DatalogProgram();
    ~DatalogProgram();
    std::string toString();
    void AddScheme(Predicate*& scheme);
    void AddFact(Predicate* fact);
    void AddQuery(Predicate* query);
    void AddRule(Rule* rule);
    std::vector<Predicate*>* GetSchemeVector();
    std::vector<Predicate*>* GetFactVector();
    std::vector<Predicate*>* GetQueryVector();
    std::vector<Rule*>* GetRuleVector();
};


#endif //PROJECT_0_DATALOGPROGRAM_H
