//
// Created by Jesse Clark on 5/15/21.
//

#include "Database.h"

Relation *Database::GetMapElement(std::string name) {
    return relationMap.at(name);
}

Database::~Database() {
    for (auto it = relationMap.begin(); it != relationMap.end(); it++) {
        delete it->second;
    }
}

void Database::AddMapElement(Relation * newRelation) {
    relationMap[newRelation->GetName()] = newRelation;
}

std::string Database::toString() {
    std::stringstream ss;
    for (auto it = relationMap.begin(); it != relationMap.end(); it++) {
        if (it == relationMap.begin()) {
            ss << it->second->toString();
        } else {
            ss << std::endl << it->second->toString();
        }
    }
    return ss.str();
}
