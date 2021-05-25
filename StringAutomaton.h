//
// Created by Jesse Clark on 5/1/21.
//

#ifndef PROJECT_0_STRINGAUTOMATON_H
#define PROJECT_0_STRINGAUTOMATON_H

#include "Automaton.h"
class StringAutomaton : public Automaton {
public:
    StringAutomaton() : Automaton(TokenType::STRING) {}  // Call the base constructor
    bool invalidString = false;
private:
    void S0(const std::string& input);
    void S1(const std::string& input);
    void S2(const std::string& input);
    void UpdateNewline(const char& curChar);
    Token* CreateToken(std::string input, int lineNumber);
};


#endif //PROJECT_0_STRINGAUTOMATON_H
