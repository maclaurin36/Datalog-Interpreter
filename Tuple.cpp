//
// Created by Jesse Clark on 5/15/21.
//

#include "Tuple.h"

bool Tuple::operator<(const Tuple &other) const {
    //DONE Compare this Tuple to other tuples for the set.  Sorted alphabetically.
    /*for (unsigned int i = 0; i < values.size(); i++) {
        if (values.at(i) < other.values.at(i)) {
            return true;
        } else if (other.values.at(i) < values.at(i)) {
            return false;
        }
    }*/
    return values < other.values;
}

std::string Tuple::toString() const{
    std::stringstream ss;
    ss << "(";
    if (values.size() > 0) {
        ss << values.at(0);
        for (unsigned int i = 1; i < values.size(); i++) {
            ss << "," << values.at(i);
        }
    }
    ss << ")";
    return ss.str();
}

void Tuple::AddElement(std::string newElement) {
    values.push_back(newElement);
}

std::string Tuple::GetValueAtIndex(int index) const {
    return values.at(index);
}

int Tuple::GetSize() {
    return values.size();
}

unsigned int Tuple::GetSize() const {
    return values.size();
}
