//
// Created by Jesse Clark on 5/7/21.
//

#ifndef PROJECT_0_PREDICATE_H
#define PROJECT_0_PREDICATE_H
#include <string>
#include <vector>
#include <sstream>
#include "Parameter.h"

class Predicate {
private:
    std::vector<Parameter*> parameterList;
    std::string IDValue;
    std::string suffix;
public:
    Predicate() {}
    Predicate(std::string ID);
    ~Predicate();
    std::string toString();
    void AddParameter(Parameter* newParameter);
    void setSuffix(std::string newSuffix);
    std::vector<Parameter*>& getList();
    std::string GetName();
};


#endif //PROJECT_0_PREDICATE_H
