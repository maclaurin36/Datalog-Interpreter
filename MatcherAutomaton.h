//
// Created by Jesse Clark on 5/1/21.
//

#ifndef PROJECT_0_MATCHERAUTOMATON_H
#define PROJECT_0_MATCHERAUTOMATON_H


#include "Automaton.h"

class MatcherAutomaton : public Automaton {
private:
    std::string toMatch;
    void Read(const std::string& input);
public:
    MatcherAutomaton(std::string toMatch, TokenType tokenType);

    void S0(const std::string &input);
};

#endif //PROJECT_0_MATCHERAUTOMATON_H
