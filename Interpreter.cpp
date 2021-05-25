//
// Created by Jesse Clark on 5/15/21.
//

#include "Interpreter.h"
Interpreter::Interpreter(DatalogProgram* program, Database* database) {
    this->program = program;
    this->database = database;
}

void Interpreter::Run() {
    // For each scheme create a relation using name and parameter values from the scheme
    for (auto it = program->GetSchemeVector()->begin(); it != program->GetSchemeVector()->end(); it++) {
        Relation* newRelation = new Relation;
        newRelation->SetName((*it)->GetName());
        Header* newHeader = new Header();
        for (auto headerIt = (*it)->getList().begin(); headerIt != (*it)->getList().end(); headerIt++) {
            newHeader->AddAttribute((*headerIt)->toString());
        }
        newRelation->SetHeader(newHeader);
        database->AddMapElement(newRelation);
    }

    // For each fact
    //      make a tuple using the values from the fact
    //      add the tuple to the relations with the same name as the fact
    for (auto it = program->GetFactVector()->begin(); it != program->GetFactVector()->end(); it++) {
        Tuple newTuple;
        for (auto parameterIt = (*it)->getList().begin(); parameterIt != (*it)->getList().end(); parameterIt++) {
            newTuple.AddElement((*parameterIt)->toString());
        }
        database->GetMapElement((*it)->GetName())->AddTuple(newTuple);
    }

    // For each query
    //      get the relation with the same name as the query
    //      select for each constant in the query
    //      select for each pair of matching variables in the query
    //      project using the positions of the variables in the query
    //      rename to match the names of variables in the query
    //      print the resulting relation
    std::stringstream ss;
    for (auto it = program->GetQueryVector()->begin(); it != program->GetQueryVector()->end(); it++) {
        Relation* printRelation = EvaluatePredicate((**it));
        if (it != program->GetQueryVector()->begin()) {
            ss << "\n";
        }
        ss << QueryResultToString((*it), printRelation);
    }
    ss << "\n";
    std::cout << ss.str();
}

Interpreter::~Interpreter() {

}

Relation *Interpreter::EvaluatePredicate(Predicate &p) {
    Relation* returnRelation = new Relation(database->GetMapElement(p.GetName()));
    for (auto it = database->GetMapElement(p.GetName())->GetRows().begin(); it != database->GetMapElement(p.GetName())->GetRows().end(); it++) {
        returnRelation->AddTuple((*it));
    }
    int i = 0;
    std::vector<int> listOfIndices;
    std::vector<std::string> listOfNames;
    std::set<std::string> setOfNames;
    std::map<std::string, int[2]> mapOfIndices;
    for (auto parameterIt = p.getList().begin(); parameterIt != p.getList().end(); parameterIt++) {
        if ((*parameterIt)->isString()) {
            returnRelation = returnRelation->Select(i, (*parameterIt)->getParameterValue());
        } else {
            if (mapOfIndices.find((*parameterIt)->toString()) != mapOfIndices.end()) {
                mapOfIndices.at((*parameterIt)->toString())[1] = i;
            } else {
                listOfIndices.push_back(i);
                mapOfIndices[(*parameterIt)->toString()][0] = i;
                mapOfIndices[(*parameterIt)->toString()][1] = -1;
            }


            if (setOfNames.find((*parameterIt)->getParameterValue()) == setOfNames.end()) {
                listOfNames.push_back((*parameterIt)->getParameterValue());
            }
            setOfNames.insert((*parameterIt)->getParameterValue());

        }
        i++;
    }
    for (auto it = mapOfIndices.begin(); it != mapOfIndices.end(); it++) {
        if ((*it).second[1] != -1) {
            returnRelation = returnRelation->Select((*it).second[0],(*it).second[1]);
        }
    }
    if (listOfIndices.size() > 0) {
        returnRelation = returnRelation->Project(&listOfIndices);
        returnRelation = returnRelation->Rename(&listOfNames);
    }

    return returnRelation;
}

std::string Interpreter::QueryResultToString(Predicate *query, Relation *relation) {
    std::stringstream ss;
    ss << query->toString() << " ";
    if (relation->GetRows().size() > 0) {
        ss << "Yes(" << relation->GetRows().size() << ")";
        for (auto rowIt = relation->GetRows().begin(); rowIt != relation->GetRows().end(); rowIt++) {
            std::set<std::string> variablesPrinted;
            int numberOfConstants = 0;
            for (unsigned int i = 0; i < query->getList().size(); i++) {
                if (!(query->getList().at(i)->isString())) {
                    if (variablesPrinted.find(query->getList().at(i)->getParameterValue()) == variablesPrinted.end()) {
                        if (i - numberOfConstants == 0) {
                            ss << "\n  ";
                        } else {
                            ss << ", ";
                        }
                        ss << query->getList().at(i)->getParameterValue() << "=" << (*rowIt).GetValueAtIndex(i - numberOfConstants);
                        variablesPrinted.insert(query->getList().at(i)->getParameterValue());
                    }
                } else {
                    numberOfConstants++;
                }
            }
        }

    } else {
        ss << "No";
    }
    return ss.str();
}
