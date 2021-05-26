//
// Created by Jesse Clark on 5/15/21.
//

#include "Relation.h"

void Relation::SetName(std::string name) {
    this->name = name;
}

void Relation::SetHeader(Header *header) {
    if (this->header != nullptr) {
        delete this->header;
    }
    this->header = header;
}

void Relation::AddTuple(Tuple newTuple) {
    rows.insert(newTuple);
}

std::string Relation::toString() {
    std::stringstream ss;
    ss << name << std::endl;
    ss << header->toString();
    for (std::set<Tuple>::iterator it = rows.begin(); it != rows.end(); it++) {
        ss << std::endl << (*it).toString();
    }
    return ss.str();
}

Relation *Relation::Select(int index, std::string value) {
    Relation* newRelation = new Relation(this);
    for (std::set<Tuple>::iterator it = rows.begin(); it != rows.end(); it++) {
        if ((*it).GetValueAtIndex(index) == value) {
            //Tuple newTuple = (*it);
            //newTuple.RemoveElementAtIndex(index);
            newRelation->AddTuple((*it));
        }
    }
    //newRelation->RemoveAttributeFromHeader(index);
    return newRelation;
}

Relation::Relation() {
    isConstant = true;
    header = nullptr;
}

std::set<Tuple> &Relation::GetRows() {
    return rows;
}

std::string Relation::GetName() {
    return name;
}

void Relation::Select(Relation*& relation, int index1, int index2) {
    for (std::set<Tuple>::iterator it = rows.begin(); it != rows.end(); it++) {
        if ((*it).GetValueAtIndex(index1) != (*it).GetValueAtIndex(index2)) {
            relation->RemoveTuple(it);
        }
    }
}

Relation *Relation::Project(std::vector<int> *listOfIndices) {
    Relation* newRelation = new Relation();
    newRelation->SetName(this->name);
    Header* newHeader = new Header();
    for (unsigned int i = 0; i < listOfIndices->size(); i++) {
        newHeader->AddAttribute(this->header->GetAttributeAtIndex(listOfIndices->at(i)));
    }
    newRelation->SetHeader(newHeader);
    for (std::set<Tuple>::iterator it = rows.begin(); it != rows.end(); it++) {
        Tuple newTuple;
        for (unsigned int i = 0; i < listOfIndices->size(); i++) {
            newTuple.AddElement((*it).GetValueAtIndex(listOfIndices->at(i)));
        }
        if (newTuple.GetSize() > 0) {
            newRelation->AddTuple(newTuple);
        }

    }
    return newRelation;
}

void Relation::Rename(Relation*& relation, std::vector<std::string> *listOfAttributes) {
    Header* newHeader = new Header();
    for (unsigned int i = 0; i < listOfAttributes->size(); i++) {
        newHeader->AddAttribute(listOfAttributes->at(i));
    }
    relation->SetHeader(newHeader);
}

Relation::~Relation() {
    if (header != nullptr) {
        delete header;
    }
}

Header* Relation::GetHeader() const {
    return header;
}

void Relation::RemoveAttributeFromHeader(unsigned int index) {
    header->RemoveAttributeAtIndex(index);
}

void Relation::SetNonConstant() {
    isConstant = false;
}

bool Relation::IsConstant() {
    return isConstant;
}

void Relation::RemoveTuple(std::set<Tuple>::iterator& it) {
    it = rows.erase(it);
}

void Relation::RemoveAllTuples() {
    rows.clear();
}

Relation::Relation(Relation *copyRelation) {
    header = nullptr;
    SetName(copyRelation->GetName());
    std::vector<std::string> oldHeader = copyRelation->header->GetAttributes();
    Header* newHeader = new Header();
    for (unsigned int i = 0; i < oldHeader.size(); i++) {
        newHeader->AddAttribute(oldHeader.at(i));
    }
    SetHeader(newHeader);
    /*std::set<Tuple> oldRows = copyRelation->GetRows();
    for (std::set<Tuple>::iterator it = oldRows.begin(); it != oldRows.end(); it++) {
        AddTuple((*it));
    }*/
}