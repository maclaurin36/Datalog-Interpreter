#ifndef TOKEN_H
#define TOKEN_H
#include <string>

enum class TokenType {
    COMMA,
    PERIOD,
    Q_MARK,
    LEFT_PAREN,
    RIGHT_PAREN,
    COLON,
    COLON_DASH,
    MULTIPLY,
    ADD,
    SCHEMES,
    FACTS,
    RULES,
    QUERIES,
    ID,
    STRING,
    COMMENT,
    UNDEFINED,
    EOF_TYPE
};

class Token
{
private:
    std::string type;
    std::string description;
    TokenType enumType;
    int line;
public:
    Token(TokenType type, std::string description, int line);
    std::string toString();
    int GetLineNumber();
    TokenType GetTokenType();
    std::string GetTokenValue();
};

#endif // TOKEN_H

