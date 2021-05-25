//
// Created by Jesse Clark on 5/1/21.
//

#include "StringAutomaton.h"

void StringAutomaton::S0(const std::string& input) {
    if (input[index] == '\'') {
        inputRead++;
        index++;
        S1(input);
    } else {
        Serr();
    }
}

void StringAutomaton::S1(const std::string& input) {
    if ((unsigned)index > input.size()) {
        inputRead--;
        invalidString = true;
    } else if (input[index] == '\'') {
        inputRead++;
        index++;
        S2(input);
    } else {
        UpdateNewline(input[index]);
        inputRead++;
        index++;
        S1(input);
    }
}

void StringAutomaton::S2(const std::string& input) {
    if (!((unsigned)index > input.size()) && input[index] == '\'') {
        inputRead++;
        index++;
        S1(input);
    }
}

void StringAutomaton::UpdateNewline(const char &curChar) {
    if (curChar == '\n') {
        newLines++;
    }
}

Token* StringAutomaton::CreateToken(std::string input, int lineNumber) {
    if (!invalidString) {
        return new Token(type, input, lineNumber);
    } else {
        invalidString = false;
        return new Token(TokenType::UNDEFINED, input, lineNumber);
    }

}