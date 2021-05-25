//
// Created by Jesse Clark on 5/15/21.
//

#include "Relation.h"

void Relation::SetName(std::string name) {
    this->name = name;
}

void Relation::SetHeader(Header *header) {
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
    return newRelation;
}

Relation::Relation() {

}

Relation::Relation(Relation *copyRelation) {
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
    return newRelation;
}

Relation *Relation::Rename(std::vector<std::string> *listOfAttributes) {
    Relation* newRelation = new Relation();
    newRelation->SetName(name);
    Header* newHeader = new Header();
    for (unsigned int i = 0; i < listOfAttributes->size(); i++) {
        newHeader->AddAttribute(listOfAttributes->at(i));
    }
    newRelation->SetHeader(newHeader);
    for (std::set<Tuple>::iterator it = rows.begin(); it != rows.end(); it++) {
        newRelation->AddTuple(*it);
    }
    return newRelation;
}

Relation::~Relation() {
    if (header != nullptr) {
        delete header;
    }
}

