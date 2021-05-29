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

    // TODO (2) Evaluate rules here (output and don't forget Query Results:)
    // Evaluate rules here (order of the input file)
        // 1) Evaluate the predicates on the right-hand side of the rule (same as queries)
        // 2) Join the relations that result
        // 3) Project the columns that appear in the head predicate (in head predicate order)
        // 4) Rename the relation to make it union compatible with the scheme in the database (look for the scheme with the same name as the rule)
        // 5) Union with the relation in the database (same name) - modifies database
        // If new tuples were added restart (use set.insert(myTuple).second which returns a boolean value if the tuple was new), pass through all rules
        // TODO (3) Figure out when to terminate (Fixed Point Algorithm)

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

void Interpreter::Test() {
    Relation* fv = database->GetMapElement("fv");
    Relation* vb = database->GetMapElement("vb");
    Relation* cd = database->GetMapElement("cd");
    Relation* bfc = database->GetMapElement("bfc");
    Relation* newRelation = fv->Join(cd);
    Relation* newRelation2 = newRelation->Join(bfc);
    /*std::list<std::pair<int,int>>* commonAttributes = new std::list<std::pair<int,int>>;
    std::set<int>* firstRelationUnique = new std::set<int>;
    std::set<int>* secondRelationUnique = new std::set<int>;
    Relation* newRelation = fv->JoinHeaderWith(vb, commonAttributes, firstRelationUnique, secondRelationUnique);
    std::vector<Tuple> tupleCollection;
    for (auto it = fv->GetRows().begin(); it != fv->GetRows().end(); it++) {
        for (auto it2 = vb->GetRows().begin(); it2 != vb->GetRows().end(); it2++) {
            if (fv->IsJoinable(*it, *it2, commonAttributes)) {
                Tuple newTuple = fv->JoinTuples(*it, *it2, commonAttributes, firstRelationUnique, secondRelationUnique);
                tupleCollection.push_back(newTuple);
                std::cout << "Hello";
            }
        }
    }*/
    //bool IsJoinable = newRelation.IsJoinable(tuple1,tuple2, commonAttributes);
    std::cout << "Hello";
    /*
    std::vector<int> something = {0};
    for (auto it = fv->GetRows().begin(); it != fv->GetRows().end(); it++) {
        for (auto it2 = vb->GetRows().begin(); it2 != vb->GetRows().end(); it2++) {
            if (fv->IsJoinable((*it), (*it2), something)) {
                std::cout << (*it).toString() << " is joinable with " << (*it2).toString() << std::endl;
            }
        }
    }*/

}
