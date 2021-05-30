//
// Created by Jesse Clark on 5/7/21.
//

#include "Rule.h"

Rule::~Rule() {
    delete headPredicate;
    for (std::vector<Predicate*>::iterator it = predicateList.begin(); it != predicateList.end(); it++) {
        delete *it;
    }
}

std::string Rule::toString() {
    std::string returnString = headPredicate->toString();
    returnString += " :- ";
    returnString += predicateList.at(0)->toString();
    for (int i = 1; (unsigned)i < predicateList.size(); i++) {
        returnString += ",";
        returnString += predicateList.at(i)->toString();
    }
    returnString += ".";
    return returnString;
}

void Rule::setHeadPredicate(Predicate* headPredicate) {
    this->headPredicate = headPredicate;
}

void Rule::addPredicate(Predicate* newPredicate) {
    predicateList.push_back(newPredicate);
}

std::vector<Predicate *> &Rule::GetPredicateList() {
    return predicateList;
}

Predicate *Rule::GetHeadPredicate() {
    return headPredicate;
}
