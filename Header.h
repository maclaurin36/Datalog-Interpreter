//
// Created by Jesse Clark on 5/15/21.
//

#ifndef PROJECT_0_HEADER_H
#define PROJECT_0_HEADER_H
#include <string>
#include <vector>
#include <sstream>
class Header {
private:
    std::vector<std::string> attributes;
public:
    Header();
    void AddAttribute(std::string newAttribute);
    std::string toString(); // Not required but may be nice;
    std::vector<std::string>& GetAttributes();
    std::string GetAttributeAtIndex(unsigned int index);
    void RemoveAttributeAtIndex(unsigned int index);
};


#endif //PROJECT_0_HEADER_H
