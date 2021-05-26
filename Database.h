//
// Created by Jesse Clark on 5/15/21.
//

#ifndef PROJECT_0_DATABASE_H
#define PROJECT_0_DATABASE_H
#include <map>
#include "Relation.h"
#include <sstream>

class Database {
private:
    std::map<std::string, Relation*> relationMap;
public:
    ~Database();
    Relation* GetMapElement(std::string name);
    void AddMapElement(Relation* newRelation);
    std::string toString();
};


#endif //PROJECT_0_DATABASE_H
