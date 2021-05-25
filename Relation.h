//
// Created by Jesse Clark on 5/15/21.
//

#ifndef PROJECT_0_RELATION_H
#define PROJECT_0_RELATION_H
#include <string>
#include "Header.h"
#include "Tuple.h"
#include <set>
class Relation {
private:
    std::string name;
    Header* header;
    std::set<Tuple> rows;
public:
    Relation();
    ~Relation();
    Relation(Relation* copyRelation);
    void SetName(std::string name);
    std::string GetName();
    void SetHeader(Header* header);
    Header* GetHeader() const;
    void AddTuple(Tuple newTuple);
    std::set<Tuple>& GetRows();
    std::string toString(); // Not required but may be nice;
    Relation* Select(int index, std::string value); // Relation column and value to select
    //TODO: Implement the other three operators
    Relation* Select(int index1, int index2); // Relation columns to select on same values
    Relation* Project(std::vector<int>* listOfIndices); // Columns to keep and their order
    Relation* Rename(std::vector<std::string>* listOfAttributes); // Defines the new header
};


#endif //PROJECT_0_RELATION_H
