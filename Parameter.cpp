//
// Created by Jesse Clark on 5/7/21.
//

#include "Parameter.h"
// parameter	->	STRING | ID

Parameter::Parameter(TokenType type, std::string value) {
    parameterType = type;
    parameterValue = value;
}

std::string Parameter::toString() {
    return parameterValue;
}

bool Parameter::isString() {
    if (parameterType == TokenType::STRING) {
        return true;
    } else {
        return false;
    }
}

std::string Parameter::getParameterValue() {
    return parameterValue;
}
