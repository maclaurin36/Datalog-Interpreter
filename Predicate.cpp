//
// Created by Jesse Clark on 5/7/21.
//

#include "Predicate.h"

// predicate	->	ID LEFT_PAREN parameter parameterList RIGHT_PAREN
// parameterList	-> 	COMMA parameter parameterList | lambda
// parameter	->	STRING | ID
std::string Predicate::toString() {
    std::string returnString = IDValue + "(";
    returnString += parameterList.at(0)->toString();
    for (int i = 1; (unsigned)i < parameterList.size(); i++) {
        returnString += ",";
        returnString += parameterList.at(i)->toString();
    }
    returnString = returnString + ")" + suffix;
    return returnString;
}

Predicate::Predicate(std::string ID) {
    IDValue = ID;
}

void Predicate::AddParameter(Parameter *newParameter) {
    parameterList.push_back(newParameter);
}

void Predicate::setSuffix(std::string newSuffix) {
    suffix = newSuffix;
}

Predicate::~Predicate() {
    for (std::vector<Parameter*>::iterator it = parameterList.begin(); it != parameterList.end(); it++) {
        delete *it;
    }
    parameterList.clear();
    IDValue = "";
    suffix = "";
}

std::vector<Parameter*>& Predicate::getList() {
   return parameterList;
}

std::string Predicate::GetName() {
    return IDValue;
}
