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
        Relation* newRelation = new Relation();
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
        delete printRelation;
    }
    ss << "\n";
    std::cout << ss.str();
}

Interpreter::~Interpreter() {

}

Relation *Interpreter::EvaluatePredicate(Predicate &p) {
    /// Create a new relation from the scheme and facts
    Relation* returnRelation = new Relation(database->GetMapElement(p.GetName()));
    for (auto it = database->GetMapElement(p.GetName())->GetRows().begin(); it != database->GetMapElement(p.GetName())->GetRows().end(); it++) {
        returnRelation->AddTuple((*it));
    }

    int i = 0;
    std::vector<int> listOfIndices;
    std::vector<std::string> listOfNames;
    std::map<std::string, int[2]> mapOfIndices;
    for (auto parameterIt = p.getList().begin(); parameterIt != p.getList().end(); parameterIt++) {
        /// If the parameter is a string, select on that column
        if ((*parameterIt)->isString()) {
            Relation* deleteRelation = returnRelation;
            returnRelation = returnRelation->Select(i, (*parameterIt)->getParameterValue());
            delete deleteRelation;
        } else {
            /// If there is a variable that is not a string, record it in the relation so that it is printed out
            returnRelation->SetNonConstant();
            /// If the variable was already recorded, record this as the second index of selection
            if (mapOfIndices.find((*parameterIt)->toString()) != mapOfIndices.end()) {
                mapOfIndices.at((*parameterIt)->toString())[1] = i;
            } else {
                /// This was the first time that the variable was seen, record it
                listOfIndices.push_back(i);
                listOfNames.push_back((*parameterIt)->getParameterValue());
                mapOfIndices[(*parameterIt)->toString()][0] = i;
                mapOfIndices[(*parameterIt)->toString()][1] = -1;
            }

        }
        i++;
    }

    /// If a variable appeared twice, select on that variable
    for (auto it = mapOfIndices.begin(); it != mapOfIndices.end(); it++) {
        if ((*it).second[1] != -1) {
            Relation* deleteRelation = returnRelation;
            returnRelation = returnRelation->Select((*it).second[0],(*it).second[1]);
            delete deleteRelation;
        }
    }

    /// Project and rename if there are variables
    if (listOfNames.size() > 0) {
        Relation* deleteRelation;
        deleteRelation = returnRelation;
        returnRelation = returnRelation->Project(&listOfIndices);
        delete deleteRelation;
        returnRelation->Rename(returnRelation, &listOfNames);
    }

    return returnRelation;
}

std::string Interpreter::QueryResultToString(Predicate *query, Relation *relation) {
    std::stringstream ss;
    ss << query->toString() << " ";
    if (relation->GetRows().size() > 0) {
        ss << "Yes(" << relation->GetRows().size() << ")";
        if (!(relation->IsConstant())) {
            for (auto rowIt = relation->GetRows().begin(); rowIt != relation->GetRows().end(); rowIt++) {
                for (unsigned int i = 0; i < (*rowIt).GetSize(); i++) {
                    if (i == 0) {
                        ss << "\n  ";
                    } else {
                        ss << ", ";
                    }
                    ss << relation->GetHeader()->GetAttributeAtIndex(i) << "=" << (*rowIt).GetValueAtIndex(i);
                }
            }
        }

    } else {
        ss << "No";
    }
    return ss.str();
}
