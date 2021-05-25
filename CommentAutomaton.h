#ifndef COLONDASHAUTOMATON_H
#define COLONDASHAUTOMATON_H

#include "Automaton.h"

class CommentAutomaton : public Automaton
{
private:
    void S1(const std::string& input);
    void S2(const std::string& input);
    void S3(const std::string& input);
    void S4(const std::string& input);
    void UpdateNewline(const char& curChar);
    bool invalidString = false;
public:
    CommentAutomaton() : Automaton(TokenType::COMMENT) {}  // Call the base constructor
    Token* CreateToken(std::string input, int lineNumber);

    void S0(const std::string& input);
};

#endif // COLONDASHAUTOMATON_H

