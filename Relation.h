//
// Created by Jesse Clark on 5/15/21.
//

#ifndef PROJECT_0_RELATION_H
#define PROJECT_0_RELATION_H
#include <string>
#include "Header.h"
#include "Tuple.h"
#include <set>
#include <list>

class Relation {
private:
    std::string name;
    Header* header;
    std::set<Tuple> rows;
    bool isConstant;
public:
    Relation();
    Relation(Relation *copyRelation);
    ~Relation();
    void SetName(std::string name);
    std::string GetName();
    void SetHeader(Header* header);
    Header* GetHeader() const;
    void AddTuple(Tuple newTuple);
    std::set<Tuple>& GetRows();
    std::string toString(); // Not required but may be nice;
    Relation* Select(int index, std::string value); // Relation column and value to select
    void SetNonConstant();
    bool IsConstant();
    Relation* Select(int index1, int index2); // Relation columns to select on same values
    Relation *Project(std::vector<int> *listOfIndices); // Columns to keep and their order
    void Rename(Relation*& relation, std::vector<std::string>* listOfAttributes); // Defines the new header
    // TODO (1) Add join and union functions here
    Relation* Join(Relation* joinTo);
    Relation* JoinHeaderWith(Relation* secondRelation, std::list<std::pair<int,int>>* commonAttributeList, std::set<int>* firstRelationUnique, std::set<int>* secondRelationUnique);
    bool IsJoinable(const Tuple &tuple1, const Tuple &tuple2, std::list<std::pair<int,int>>* commonAttributes);
    Tuple JoinTuples(const Tuple &tuple1, const Tuple &tuple2, const std::list<std::pair<int, int>> *commonAttributes, std::set<int>* firstRelationUnique, std::set<int>* secondRelationUnique);
    bool Union(Relation* incomingRelation);
    // Join - another relation to join with, returns the joined relation, always possible - Cartesian Product?
    // Union - the one you are inside is the database relation, the one you pass in is the other one
};


#endif //PROJECT_0_RELATION_H
