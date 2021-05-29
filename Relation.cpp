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

Relation *Relation::Join(Relation *secondRelation) {
    std::list<std::pair<int,int>>* commonAttributeList = new std::list<std::pair<int,int>>;
    std::set<int>* firstRelationUnique = new std::set<int>;
    std::set<int>* secondRelationUnique = new std::set<int>;
    Relation* newRelation = this->JoinHeaderWith(secondRelation, commonAttributeList, firstRelationUnique, secondRelationUnique);

    if (newRelation->GetHeader()->GetAttributes().size() < header->GetAttributes().size() + secondRelation->GetHeader()->GetAttributes().size()) {
        std::set<Tuple> secondRows = secondRelation->GetRows();
        for (auto itR1Tuple = rows.begin(); itR1Tuple != rows.end(); itR1Tuple++) {
            for (auto itR2Tuple = secondRows.begin(); itR2Tuple != secondRows.end(); itR2Tuple++) {
                if (IsJoinable((*itR1Tuple), (*itR2Tuple), commonAttributeList)) {
                    newRelation->AddTuple(JoinTuples((*itR1Tuple), *itR2Tuple, commonAttributeList, firstRelationUnique, secondRelationUnique));
                }
            }
        }
    }
    delete commonAttributeList;
    delete firstRelationUnique;
    delete secondRelationUnique;
    return newRelation;
}

bool Relation::Union(Relation *incomingRelation) {
    std::set<Tuple> incomingTuples = incomingRelation->GetRows();
    bool tupleInserted = false;
    for (auto it = incomingTuples.begin(); it != incomingTuples.end(); it++) {
        tupleInserted = rows.insert((*it)).second;
    }
    return tupleInserted;
}

// commonAttributes contains the common attributes in the order (->,parameter), and the Relation's header is common,uncommon->,uncommonParam
Relation* Relation::JoinHeaderWith(Relation* secondRelation, std::list<std::pair<int,int>>* commonAttributeList, std::set<int>* firstRelationUnique, std::set<int>* secondRelationUnique) {
    std::vector<std::string> attributes1 = header->GetAttributes();
    std::vector<std::string> attributes2 = secondRelation->GetHeader()->GetAttributes();

    //commonAttributeList->clear();
    //firstRelationUnique->clear();
    //secondRelationUnique->clear();

    bool uniqueValue = true;
    std::set<int> commonAttributes;
    Header* newHeader = new Header();

    for (unsigned int j = 0; j < attributes2.size(); j++) {
        for (unsigned int i = 0; i < attributes1.size(); i++) {
            if (attributes1.at(i) == attributes2.at(j)) {
                newHeader->AddAttribute(attributes1.at(i));
                commonAttributes.insert(i);
                commonAttributeList->push_back(std::pair<int,int>(i,j));
                uniqueValue = false;
            }
        }
        if (uniqueValue) {
            secondRelationUnique->insert(j);
        } else {
            uniqueValue = true;
        }
    }

    for (unsigned int i = 0; i < attributes1.size(); i++) {
        if (commonAttributes.find(i) == commonAttributes.end()) {
            firstRelationUnique->insert(i);
            newHeader->AddAttribute(attributes1.at(i));
        }
    }

    for (unsigned int j = 0; j < attributes2.size(); j++) {
        if (secondRelationUnique->find(j) != secondRelationUnique->end()) {
            newHeader->AddAttribute(attributes2.at(j));
        }
    }
    Relation* newRelation = new Relation();
    newRelation->SetHeader(newHeader);

    return newRelation;
}

/// Assuming the Tuples are joinable
Tuple Relation::JoinTuples(const Tuple &tuple1, const Tuple &tuple2, const std::list<std::pair<int, int>> *commonAttributes, std::set<int>* firstRelationUnique, std::set<int>* secondRelationUnique) {
    Tuple newTuple;
    bool isJoinable = true;
    for (auto it = commonAttributes->begin(); it != commonAttributes->end(); it++) {
        newTuple.AddElement(tuple1.GetValueAtIndex((*it).first));
    }
    for (unsigned int i = 0; i < tuple1.GetSize(); i++) {
        if (firstRelationUnique->find(i) != firstRelationUnique->end()) {
            newTuple.AddElement(tuple1.GetValueAtIndex(i));
        }
    }
    for (unsigned int i = 0; i < tuple2.GetSize(); i++) {
        if (secondRelationUnique->find(i) != secondRelationUnique->end()) {
            newTuple.AddElement(tuple2.GetValueAtIndex(i));
        }
    }
    return newTuple;
}

bool Relation::IsJoinable(const Tuple &tuple1, const Tuple &tuple2, std::list<std::pair<int,int>>* commonAttributes) {
    for (auto it = commonAttributes->begin(); it != commonAttributes->end(); it++) {
        if (tuple1.GetValueAtIndex((*it).first) != tuple2.GetValueAtIndex((*it).second)) {
            return false;
        }
    }
    return true;
}
