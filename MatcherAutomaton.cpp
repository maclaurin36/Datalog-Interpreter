//
// Created by Jesse Clark on 5/1/21.
//

#include "MatcherAutomaton.h"

MatcherAutomaton::MatcherAutomaton(std::string toMatch, TokenType tokenType) {
    this->type = tokenType;
    this->toMatch = toMatch;
}

// This is more or less a dummy state that must be inherited rather than an actual state
void MatcherAutomaton::S0(const std::string& input) {
    this->Read(input);
}

void MatcherAutomaton::Read(const std::string& input) {
    bool isMatch = true;
    inputRead = 0;
    for (int i = 0; i < (int)toMatch.size() && isMatch; i++) {
        if (input[i] != toMatch[i]) {
            isMatch = false;
        }
    }
    if (isMatch) {
        inputRead = toMatch.size();
    }
}