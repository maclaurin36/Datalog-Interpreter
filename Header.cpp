//
// Created by Jesse Clark on 5/15/21.
//

#include "Header.h"

void Header::AddAttribute(std::string newAttribute) {
    attributes.push_back(newAttribute);
}

Header::Header() {

}

std::string Header::toString() {
    std::stringstream ss;
    ss << "(";
    if (attributes.size() > 0) {
        ss << attributes.at(0);
        for (unsigned int i = 1; i < attributes.size(); i++) {
            ss << "," << attributes.at(i);
        }
    }
    ss << ")";
    return ss.str();
}

std::vector<std::string> &Header::GetAttributes() {
    return attributes;
}

std::string Header::GetAttributeAtIndex(unsigned int index) {
    return attributes.at(index);
}

void Header::RemoveAttributeAtIndex(unsigned int index) {
    attributes.erase(attributes.begin() + index);
}
