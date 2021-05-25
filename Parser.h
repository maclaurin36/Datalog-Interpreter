//
// Created by Jesse Clark on 5/6/21.
//

#ifndef PROJECT_0_PARSER_H
#define PROJECT_0_PARSER_H
#include "Token.h"
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"
#include "DatalogProgram.h"
#include <vector>
#include <iostream>

class Parser {
private:
    std::vector<Token*>& tokens;
    void ParseDatalogProgram();
    void ParseSchemeList();
    void ParseFactList();
    void ParseRuleList();
    void ParseQueryList();
    void ParseScheme();
    void ParseFact();
    void ParseRule();
    void ParseQuery();
    Predicate* ParseHeadPredicate();
    Predicate* ParsePredicate();
    void ParsePredicateList(Rule*& parentRule);
    void ParseParameterList(Predicate*& parameterOwner);
    void ParseStringList(Predicate*& stringOwner);
    void ParseIdList(Predicate*& idListOwner);
    void ParseParameter(Predicate*& parameterOwner);
    void ParseTerminal(TokenType matchType);
    void ParseEOF();
    int tokenIndex;
    DatalogProgram* currentProgram;
public:
    Parser(std::vector<Token*>& tokenParam, DatalogProgram* program) : tokens(tokenParam) {tokenIndex = 0; currentProgram = program;}
    ~Parser() {}
    void Run();
};


#endif //PROJECT_0_PARSER_H
