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
            newRelation->AddTuple((*it));
        }
    }
    if (!isConstant) {
        newRelation->SetNonConstant();
    }
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

Relation *Relation::Select(int index1, int index2) {
    Relation* newRelation = new Relation(this);
    for (std::set<Tuple>::iterator it = rows.begin(); it != rows.end(); it++) {
        if ((*it).GetValueAtIndex(index1) == (*it).GetValueAtIndex(index2)) {
            newRelation->AddTuple((*it));
        }
    }
    if (!isConstant) {
        newRelation->SetNonConstant();
    }
    return newRelation;
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
    if (!isConstant) {
        newRelation->SetNonConstant();
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

void Relation::SetNonConstant() {
    isConstant = false;
}

bool Relation::IsConstant() {
    return isConstant;
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
    isConstant = copyRelation->IsConstant();
}

Relation *Relation::Join(Relation *joinTo) {

    return nullptr;
}

bool Relation::Union(Relation *incomingRelation) {
    std::set<Tuple> incomingTuples = incomingRelation->GetRows();
    bool tupleInserted = false;
    for (auto it = incomingTuples.begin(); it != incomingTuples.end(); it++) {
        tupleInserted = rows.insert((*it)).second;
    }
    return tupleInserted;
}
