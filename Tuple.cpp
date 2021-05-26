//
// Created by Jesse Clark on 5/15/21.
//

#include "Tuple.h"

bool Tuple::operator<(const Tuple &other) const {
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