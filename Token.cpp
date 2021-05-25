#include "Token.h"

Token::Token(TokenType type, std::string description, int line) {
    switch (type) {
        case TokenType::COLON:
            this->type = "COLON";
            break;
        case TokenType::COLON_DASH:
            this->type = "COLON_DASH";
            break;
        case TokenType::UNDEFINED:
            this->type = "UNDEFINED";
            break;
        case TokenType::COMMA:
            this->type = "COMMA";
            break;
        case TokenType::PERIOD:
            this->type = "PERIOD";
            break;
        case TokenType::Q_MARK:
            this->type = "Q_MARK";
            break;
        case TokenType::LEFT_PAREN:
            this->type = "LEFT_PAREN";
            break;
        case TokenType::RIGHT_PAREN:
            this->type = "RIGHT_PAREN";
            break;
        case TokenType::MULTIPLY:
            this->type = "MULTIPLY";
            break;
        case TokenType::ADD:
            this->type = "ADD";
            break;
        case TokenType::SCHEMES:
            this->type = "SCHEMES";
            break;
        case TokenType::FACTS:
            this->type = "FACTS";
            break;
        case TokenType::RULES:
            this->type = "RULES";
            break;
        case TokenType::QUERIES:
            this->type = "QUERIES";
            break;
        case TokenType::ID:
            this->type = "ID";
            break;
        case TokenType::STRING:
            this->type = "STRING";
            break;
        case TokenType::COMMENT:
            this->type = "COMMENT";
            break;
        case TokenType::EOF_TYPE:
            this->type = "EOF";
            break;
    }
    this->description = description;
    this->line = line;
    this->enumType = type;
}

std::string Token::toString() {
    std::string returnString = "(" + this->type + ",\"" + this->description + "\"," + std::to_string(this->line) + ")";
    return returnString;
}

int Token::GetLineNumber() {
    return line;
}

TokenType Token::GetTokenType() {
    return this->enumType;
}

std::string Token::GetTokenValue() {
    return this->description;
}
