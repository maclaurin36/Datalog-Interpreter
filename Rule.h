//
// Created by Jesse Clark on 5/7/21.
//

#ifndef PROJECT_0_RULE_H
#define PROJECT_0_RULE_H
#include <string>
#include "Predicate.h"

class Rule {
private:
    Predicate* headPredicate;
    std::vector<Predicate*> predicateList;
public:
    ~Rule();
    std::string toString();
    void setHeadPredicate(Predicate* headPredicate);
    void addPredicate(Predicate* newPredicate);
};


#endif //PROJECT_0_RULE_H
