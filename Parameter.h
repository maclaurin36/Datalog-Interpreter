//
// Created by Jesse Clark on 5/7/21.
//

#ifndef PROJECT_0_PARAMETER_H
#define PROJECT_0_PARAMETER_H
#include "Token.h"
#include <string>

class Parameter {
private:
    TokenType parameterType;
    std::string parameterValue;
public:
    Parameter(TokenType type, std::string value);
    std::string toString();
    bool isString();
    std::string getParameterValue();
};


#endif //PROJECT_0_PARAMETER_H
