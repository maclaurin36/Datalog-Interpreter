//
// Created by Jesse Clark on 5/7/21.
//

#include "DatalogProgram.h"

DatalogProgram::DatalogProgram() {

}

DatalogProgram::~DatalogProgram() {
    for (std::vector<Predicate*>::iterator it = schemes.begin(); it != schemes.end(); it++) {
        delete *it;
    }
    for (std::vector<Predicate*>::iterator it = facts.begin(); it != facts.end(); it++) {
        delete *it;
    }
    for (std::vector<Rule*>::iterator it = rules.begin(); it != rules.end(); it++) {
        delete *it;
    }
    for (std::vector<Predicate*>::iterator it = queries.begin(); it != queries.end(); it++) {
        delete *it;
    }
}

std::string DatalogProgram::toString() {
    std::stringstream returnStringstream;

    returnStringstream << "Schemes(" << schemes.size() << "):" << std::endl;
    for (std::vector<Predicate*>::iterator it = schemes.begin(); it != schemes.end(); it++) {
        returnStringstream << "  ";
        returnStringstream << (*it)->toString();
        returnStringstream << std::endl;
    }

    returnStringstream << "Facts(" << facts.size() << "):" << std::endl;
    if (facts.size() > 0) {
        for (std::vector<Predicate*>::iterator it = facts.begin(); it != facts.end(); it++) {
            returnStringstream << "  ";
            returnStringstream << (*it)->toString();
            returnStringstream << std::endl;
        }
    }

    returnStringstream << "Rules(" << rules.size() << "):" << std::endl;
    if (rules.size() > 0) {
        for (std::vector<Rule*>::iterator it = rules.begin(); it != rules.end(); it++) {
            returnStringstream << "  ";
            returnStringstream << (*it)->toString();
            returnStringstream << std::endl;
        }
    }

    returnStringstream << "Queries(" << queries.size() << "):" << std::endl;
    if (queries.size() > 0) {
        for (std::vector<Predicate*>::iterator it = queries.begin(); it != queries.end(); it++) {
            returnStringstream << "  ";
            returnStringstream << (*it)->toString();
            returnStringstream << std::endl;
        }
    }

    returnStringstream << "Domain(" << domain.size() << "):" << std::endl;
    if (domain.size() > 0) {
        for (std::set<std::string>::iterator it = domain.begin(); it != domain.end(); it++) {
            //if (!(it == domain.begin())) {

            //}
            returnStringstream << "  ";
            returnStringstream << "'" << (*it) << "'";
            returnStringstream << std::endl;
        }
    }

    return returnStringstream.str();
}

void DatalogProgram::AddScheme(Predicate *&scheme) {
    schemes.push_back(scheme);
}

void DatalogProgram::AddFact(Predicate *fact) {
    facts.push_back(fact);
    std::vector<Parameter*> factStrings = fact->getList();
    for (std::vector<Parameter*>::iterator it = factStrings.begin(); it < factStrings.end(); it++) {
        domain.insert((*it)->toString().substr(1,(*it)->toString().length() - 2));
    }
}

void DatalogProgram::AddQuery(Predicate *query) {
    queries.push_back(query);
}

void DatalogProgram::AddRule(Rule *rule) {
    rules.push_back(rule);
}

std::vector<Predicate *> *DatalogProgram::GetFactVector() {
    return &facts;
}

std::vector<Predicate *> *DatalogProgram::GetSchemeVector() {
    return &schemes;
}

std::vector<Predicate *> *DatalogProgram::GetQueryVector() {
    return &queries;
}

std::vector<Rule *> *DatalogProgram::GetRuleVector() {
    return &rules;
}
