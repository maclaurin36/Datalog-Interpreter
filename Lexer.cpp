#include "Lexer.h"
#include "CommentAutomaton.h"
#include "MatcherAutomaton.h"
#include "StringAutomaton.h"
#include "IDAutomaton.h"
#include <iostream>

Lexer::Lexer() {
    CreateAutomata();
}

Lexer::~Lexer() {
    for(std::vector<Automaton*>::iterator it = automata.begin(); it < automata.end(); it++) {
        delete (*it);
    }
    for(std::vector<Token*>::iterator it = tokens.begin(); it < tokens.end(); it++) {
        delete (*it);
    }
    automata.clear();
    tokens.clear();
}

void Lexer::CreateAutomata() {
    automata.push_back(new MatcherAutomaton(",", TokenType::COMMA));
    automata.push_back(new MatcherAutomaton(".", TokenType::PERIOD));
    automata.push_back(new MatcherAutomaton("?", TokenType::Q_MARK));
    automata.push_back(new MatcherAutomaton("(", TokenType::LEFT_PAREN));
    automata.push_back(new MatcherAutomaton(")", TokenType::RIGHT_PAREN));
    automata.push_back(new MatcherAutomaton(":", TokenType::COLON));
    automata.push_back(new MatcherAutomaton(":-", TokenType::COLON_DASH));
    automata.push_back(new MatcherAutomaton("*", TokenType::MULTIPLY));
    automata.push_back(new MatcherAutomaton("+", TokenType::ADD));
    automata.push_back(new MatcherAutomaton("Schemes", TokenType::SCHEMES));
    automata.push_back(new MatcherAutomaton("Facts", TokenType::FACTS));
    automata.push_back(new MatcherAutomaton("Rules", TokenType::RULES));
    automata.push_back(new MatcherAutomaton("Queries", TokenType::QUERIES));
    automata.push_back(new StringAutomaton());
    automata.push_back(new CommentAutomaton());
    automata.push_back(new IDAutomaton());
}

void Lexer::Run(std::string& input) {

    int lineNumber = 1;

    while (input.size() > 0) {
        int maxRead = 0;
        Automaton* maxAutomaton = automata.at(0);

        while (input.size() != 0 && (input.at(0) == '\n' || input.at(0) == ' ' || input.at(0) == '\t')) {
            if (input.at(0) == '\n') {
                lineNumber++;
            }
            input = input.substr(1, input.size() - 1);
        }

        if (input.size() > 0) {
            // Here is the "Parallel" part of the algorithm
            //  Each automaton runs with the same input
            for (std::vector<Automaton *>::iterator it = automata.begin(); it != automata.end(); it++) {
                int inputRead = (*it)->Start(input);
                if (inputRead > maxRead) {
                    maxRead = inputRead;
                    maxAutomaton = *it;
                }
            }
            // Here is the "Max" part of the algorithm
            if (maxRead > 0) {
                std::string newInput = input.substr(0, maxRead);
                Token *newToken = maxAutomaton->CreateToken(newInput, lineNumber);
                lineNumber += maxAutomaton->NewLinesRead();
                if (newToken->GetTokenType() == TokenType::COMMENT) {
                    delete newToken;
                } else {
                    tokens.push_back(newToken);
                }
            }
            // No automaton accepted input
            // Create single character undefined token
            else {
                maxRead = 1;
                Token *newToken = new Token(TokenType::UNDEFINED, input.substr(0, 1), lineNumber);
                tokens.push_back(newToken);
            }
            // Update `input` by removing characters read to create Token
            input = input.substr(maxRead, input.size() - maxRead);
        }
    }
    // Push on the EOF token
    Token *newToken = new Token(TokenType::EOF_TYPE,"", lineNumber);
    tokens.push_back(newToken);

}

void Lexer::PrintTokenVector() {
    for (std::vector<Token*>::iterator it = tokens.begin(); it != tokens.end(); it++) {
        std::cout << (*it)->toString() << std::endl;
    }
    std::cout << "Total Tokens = " << tokens.size();
}

std::vector<Token *> &Lexer::GetTokenVector() {
    return tokens;
}
