//
// Created by Jesse Clark on 5/6/21.
//

#include "Parser.h"
#include <iostream>
/*
 * Set up the Parser class to check the syntax and print out success/failure appropriately
 * Create the DatalogProgram, Predicate, Rule, and Parameter classes (I'm not sure what these are for again, look it up once more)
 *
 */

//datalogProgram	->	SCHEMES COLON scheme schemeList FACTS COLON factList RULES COLON ruleList QUERIES COLON query queryList EOF
void Parser::ParseDatalogProgram() {
    ParseTerminal(TokenType::SCHEMES);
    ParseTerminal(TokenType::COLON);
    ParseScheme();
    ParseSchemeList();
    ParseTerminal(TokenType::FACTS);
    ParseTerminal(TokenType::COLON);
    ParseFactList();
    ParseTerminal(TokenType::RULES);
    ParseTerminal(TokenType::COLON);
    ParseRuleList();
    ParseTerminal(TokenType::QUERIES);
    ParseTerminal(TokenType::COLON);
    ParseQuery();
    ParseQueryList();
    ParseEOF();
}

//schemeList	->	scheme schemeList | lambda
void Parser::ParseSchemeList() {
    if (tokens.at(tokenIndex)->GetTokenType() == TokenType::ID) {
        ParseScheme();
        ParseSchemeList();
    }
}

//factList	->	fact factList | lambda
void Parser::ParseFactList() {
    if (tokens.at(tokenIndex)->GetTokenType() == TokenType::ID) {
        ParseFact();
        ParseFactList();
    }
}

//ruleList	->	rule ruleList | lambda
void Parser::ParseRuleList() {
    if (tokens.at(tokenIndex)->GetTokenType() == TokenType::ID) {
        ParseRule();
        ParseRuleList();
    }
}

//queryList	->	query queryList | lambda
void Parser::ParseQueryList() {
    if (tokens.at(tokenIndex)->GetTokenType() == TokenType::ID) {
        ParseQuery();
        ParseQueryList();
    }
}

//scheme   	-> 	ID LEFT_PAREN ID idList RIGHT_PAREN
void Parser::ParseScheme() {
    ParseTerminal(TokenType::ID);
    Predicate* newScheme = new Predicate(tokens.at(tokenIndex - 1)->GetTokenValue());
    ParseTerminal(TokenType::LEFT_PAREN);
    ParseTerminal(TokenType::ID);
    Parameter* idParameter = new Parameter(TokenType::ID, tokens.at(tokenIndex - 1)->GetTokenValue());
    newScheme->AddParameter(idParameter);
    ParseIdList(newScheme);
    ParseTerminal(TokenType::RIGHT_PAREN);
    currentProgram->AddScheme(newScheme);
}

//fact    	->	ID LEFT_PAREN STRING stringList RIGHT_PAREN PERIOD
void Parser::ParseFact() {
    ParseTerminal(TokenType::ID);
    Predicate* newFact = new Predicate(tokens.at(tokenIndex - 1)->GetTokenValue());
    ParseTerminal(TokenType::LEFT_PAREN);
    ParseTerminal(TokenType::STRING);
    Parameter* newParam = new Parameter(TokenType::STRING, tokens.at(tokenIndex - 1)->GetTokenValue());
    newFact->AddParameter(newParam);
    ParseStringList(newFact);
    ParseTerminal(TokenType::RIGHT_PAREN);
    ParseTerminal(TokenType::PERIOD);
    newFact->setSuffix(".");
    currentProgram->AddFact(newFact);
}

//rule    	->	headPredicate COLON_DASH predicate predicateList PERIOD
void Parser::ParseRule() {
    Rule* newRule = new Rule();
    newRule->setHeadPredicate(ParseHeadPredicate());
    ParseTerminal(TokenType::COLON_DASH);
    newRule->addPredicate(ParsePredicate());
    ParsePredicateList(newRule);
    ParseTerminal(TokenType::PERIOD);
    currentProgram->AddRule(newRule);
}

//query	        ->      predicate Q_MARK
void Parser::ParseQuery() {
    Predicate* newPredicate = ParsePredicate();
    ParseTerminal(TokenType::Q_MARK);
    newPredicate->setSuffix("?");
    currentProgram->AddQuery(newPredicate);
}

//headPredicate	->	ID LEFT_PAREN ID idList RIGHT_PAREN
Predicate* Parser::ParseHeadPredicate() {
    ParseTerminal(TokenType::ID);
    Predicate* newHeadPredicate = new Predicate(tokens.at(tokenIndex - 1)->GetTokenValue());
    ParseTerminal(TokenType::LEFT_PAREN);
    ParseTerminal(TokenType::ID);
    Parameter* idParameter = new Parameter(TokenType::ID, tokens.at(tokenIndex - 1)->GetTokenValue());
    newHeadPredicate->AddParameter(idParameter);
    ParseIdList(newHeadPredicate);
    ParseTerminal(TokenType::RIGHT_PAREN);
    return newHeadPredicate;
}

//predicate	->	ID LEFT_PAREN parameter parameterList RIGHT_PAREN
Predicate* Parser::ParsePredicate() {
    ParseTerminal(TokenType::ID);
    Predicate* newPredicate = new Predicate(tokens.at(tokenIndex - 1)->GetTokenValue());
    ParseTerminal(TokenType::LEFT_PAREN);
    ParseParameter(newPredicate);
    ParseParameterList(newPredicate);
    ParseTerminal(TokenType::RIGHT_PAREN);
    return newPredicate;
}

//predicateList	->	COMMA predicate predicateList | lambda
void Parser::ParsePredicateList(Rule*& parentRule) {
    if (tokens.at(tokenIndex)->GetTokenType() == TokenType::COMMA) {
        ParseTerminal(TokenType::COMMA);
        parentRule->addPredicate(ParsePredicate());
        ParsePredicateList(parentRule);
    }
}

//parameterList	-> 	COMMA parameter parameterList | lambda
void Parser::ParseParameterList(Predicate*& parameterOwner) {
    if (tokens.at(tokenIndex)->GetTokenType() == TokenType::COMMA) {
        ParseTerminal(TokenType::COMMA);
        ParseParameter(parameterOwner);
        ParseParameterList(parameterOwner);
    }
}

//stringList	-> 	COMMA STRING stringList | lambda
void Parser::ParseStringList(Predicate*& stringOwner) {
    if (tokens.at(tokenIndex)->GetTokenType() == TokenType::COMMA) {
        ParseTerminal(TokenType::COMMA);
        ParseTerminal(TokenType::STRING);
        Parameter* newParam = new Parameter(TokenType::STRING, tokens.at(tokenIndex - 1)->GetTokenValue());
        stringOwner->AddParameter(newParam);
        ParseStringList(stringOwner);
    }
}

//idList  	-> 	COMMA ID idList | lambda
void Parser::ParseIdList(Predicate*& idListOwner) {
    if (tokens.at(tokenIndex)->GetTokenType() == TokenType::COMMA) {
        ParseTerminal(TokenType::COMMA);
        ParseTerminal(TokenType::ID);
        Parameter* newIdParameter = new Parameter(TokenType::ID, tokens.at(tokenIndex - 1)->GetTokenValue());
        idListOwner->AddParameter(newIdParameter);
        ParseIdList(idListOwner);
    }
}

//parameter	->	STRING | ID
void Parser::ParseParameter(Predicate*& parameterOwner) {
    if (tokens.at(tokenIndex)->GetTokenType() == TokenType::STRING) {
        ParseTerminal(TokenType::STRING);
        Parameter* newParameter = new Parameter(TokenType::STRING, tokens.at(tokenIndex - 1)->GetTokenValue());
        parameterOwner->AddParameter(newParameter);
    } else if (tokens.at(tokenIndex)->GetTokenType() == TokenType::ID) {
        ParseTerminal(TokenType::ID);
        Parameter* newParameter = new Parameter(TokenType::ID, tokens.at(tokenIndex - 1)->GetTokenValue());
        parameterOwner->AddParameter(newParameter);
    } else {
        throw tokens.at(tokenIndex)->toString();
    }
}

void Parser::ParseTerminal(TokenType matchType) {
    if (tokens.at(tokenIndex)->GetTokenType() == matchType) {
        tokenIndex++;
    } else {
        throw (tokens.at(tokenIndex)->toString());
    }
}

void Parser::Run() {
    try {
        ParseDatalogProgram();
        //std::cout << currentProgram->toString();
        /*//Debugging
        Parameter* param = new Parameter(TokenType::ID, "AnotherID");
        Parameter* param2 = new Parameter(TokenType::STRING, "'This is a string'");
        Predicate* pred = new Predicate("THISisTHEid");
        pred->AddParameter(param);
        pred->AddParameter(param2);
        Parameter* param3 = new Parameter(TokenType::STRING, "'otherString'");
        Parameter* param6 = new Parameter(TokenType::STRING, "'otherID2'");
        Parameter* param4 = new Parameter(TokenType::STRING, "'listID'");
        Parameter* param5 = new Parameter(TokenType::STRING, "'listID2'");
        Predicate* headPred = new Predicate("headPredID");
        headPred->AddParameter(param4);
        headPred->AddParameter(param5);
        Predicate* tailPred = new Predicate("TailPredID");
        tailPred->AddParameter(param3);
        tailPred->AddParameter(param6);
        //Rule* newRule = new Rule();
        //newRule->setHeadPredicate(headPred);
        //newRule->addPredicate(tailPred);
        //newRule->addPredicate(pred);
        headPred->setSuffix(".");
        tailPred->setSuffix(".");
        DatalogProgram* newProg = new DatalogProgram();
        newProg->AddFact(headPred);
        newProg->AddFact(tailPred);
        //newProg->AddRule(newRule);
        std::cout << newProg->toString() << std::endl;
        //std::cout << pred->toString() << std::endl;
        delete param;
        */
    }
    catch (std::string error) {
        std::cout << "Failure!" << std::endl;
        std::cout << "  " << error << std::endl;
    }

}

void Parser::ParseEOF() {
    ParseTerminal(TokenType::EOF_TYPE);
    //std::cout << "Success!" << std::endl;
}



