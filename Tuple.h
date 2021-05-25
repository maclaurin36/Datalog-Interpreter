//
// Created by Jesse Clark on 5/15/21.
//

#ifndef PROJECT_0_TUPLE_H
#define PROJECT_0_TUPLE_H
#include <string>
#include <vector>
#include <sstream>

class Tuple {
private:
    std::vector<std::string> values;
public:
    void AddElement(std::string newElement);
    std::string GetValueAtIndex(int index) const;
    bool operator< (const Tuple & other) const;
    int GetSize();
    unsigned int GetSize() const;
    std::string toString() const; // Not required but may be nice;
};


#endif //PROJECT_0_TUPLE_H
