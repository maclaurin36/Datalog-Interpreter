//
// Created by Jesse Clark on 5/2/21.
//

#ifndef PROJECT_0_IDAUTOMATON_H
#define PROJECT_0_IDAUTOMATON_H

#include "Automaton.h"

class IDAutomaton : public Automaton {
private:
    void S1(const std::string& input);
public:
    IDAutomaton() : Automaton(TokenType::ID) {}  // Call the base constructor
    void S0(const std::string& input);
};


#endif //PROJECT_0_IDAUTOMATON_H
