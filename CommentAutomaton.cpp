#include "CommentAutomaton.h"

void CommentAutomaton::S0(const std::string &input) {
    if (input[index] == '#') {
        inputRead++;
        index++;
        S1(input);
    } else {
        Serr();
    }
}

void CommentAutomaton::S1(const std::string &input) {
    if (input[index] == '|') {
        inputRead++;
        index++;
        S2(input);
    } else {
        S4(input);
    }
}

void CommentAutomaton::S2(const std::string &input) {
    if ((unsigned)index > input.size()) {
        inputRead--;
        invalidString = true;
    } else if (input[index] == '|') {
        inputRead++;
        index++;
        S3(input);
    } else {
        inputRead++;
        index++;
        UpdateNewline(input[index]);
        S2(input);
    }
}

void CommentAutomaton::S3(const std::string &input) {
    if ((unsigned)index > input.size()) {
        inputRead--;
        invalidString = true;
    } else if (input[index] == '#') {
        inputRead++;
        index++;
    } else {
        UpdateNewline(input[index]);
        S2(input);
    }
}

void CommentAutomaton::UpdateNewline(const char &curChar) {
    if (curChar == '\n') {
        newLines++;
    }
}

Token* CommentAutomaton::CreateToken(std::string input, int lineNumber) {
    if (invalidString) {
        invalidString = false;
        return new Token(TokenType::UNDEFINED, input, lineNumber);
    } else {
        return new Token(type, input, lineNumber);
    }
}

void CommentAutomaton::S4(const std::string &input) {
    if ((unsigned)index > input.size()) {
        inputRead--;
    } else if (input[index] != '\n') {
        index++;
        inputRead++;
        S4(input);
    }
}
