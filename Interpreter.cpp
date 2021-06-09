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

    /*
     * TODO List of Stuff
     *      1 - Create the graph class
     *      2 - Run the strongly connected component algorithm to get sets of the strongly connected components
     *      3 - Evaluate the rules in order of the strongly connected components found
     */
    Graph* graph = new Graph(program->GetRuleVector(), false);
    Graph* reverseGraph = new Graph(program->GetRuleVector(), true);
    std::cout << "Dependency Graph" << std::endl;
    std::cout << graph->toString() << std::endl;
    // TODO remember to delete these
    std::stack<int>* postOrder = reverseGraph->DepthFirstSearchForest();
    std::vector<std::set<int>*>* SCCVector = graph->DepthFirstSearchForestSCC(postOrder);
    /* TODO (2) Figure out the strongly connected components of the rules
     *      2.1 Create the graph of the rules G
     *      2.2 Create the reverse graph Gr of G
     *      2.3 Run DFSF on Gr and record the postorder of the nodes
     *      2.4 Reverse the postorder
     *      2.5 Run DFSF on G using the reversed postorder
    */

    /*
     * TODO (3) Evaluate the rules in order of the strongly connected components found
     *      NOTE Trivial node that doesn't depend on itself should only have one pass
     */
    std::cout << std::endl;
    std::cout << "Rule Evaluation" << std::endl;
    for (unsigned int i = 0; i < SCCVector->size(); i++) {
        bool isTrivial = false;
        if (SCCVector->at(i)->size() == 1) {
            auto it = SCCVector->at(i)->begin();
            isTrivial = graph->IsTrivial((*it));
        }
        EvaluateSCC(SCCVector->at(i), isTrivial);
    }
    /*bool tupleAdded = false;
    int numberOfPasses = 0;
    do {
        tupleAdded = false;
        // Evaluate rules here (order of the input file)
        std::vector<Rule*>* ruleVector = program->GetRuleVector();
        for (unsigned int i = 0; i < ruleVector->size(); i++) {
            // 1) Evaluate the predicates on the right-hand side of the rule (same as queries)
            Rule* currentRule = ruleVector->at(i);
            std::cout << currentRule->toString() << std::endl;
            std::vector<Predicate*> predicateList = currentRule->GetPredicateList(); // by reference
            std::vector<Relation*> relationList;
            for (unsigned int j = 0; j < predicateList.size(); j++) {
                relationList.push_back(EvaluatePredicate(*predicateList.at(j)));
            }
            // 2) Join the relations that result
            Relation* joiningRelation = relationList.at(0);
            for (unsigned int j = 1; j < relationList.size(); j++) {
                Relation* deleteRelation = joiningRelation;
                joiningRelation = joiningRelation->Join(relationList.at(j));
                delete deleteRelation;
            }
            // 3) Project the columns that appear in the head predicate (in head predicate order)
            {
                std::vector<int> listOfIndices;
                for (unsigned int j = 0; j < currentRule->GetHeadPredicate()->getList().size(); j++) {
                    for (unsigned int k = 0; k < joiningRelation->GetHeader()->GetAttributes().size(); k++) {
                        if (currentRule->GetHeadPredicate()->getList().at(j)->getParameterValue() == joiningRelation->GetHeader()->GetAttributes().at(k)) {
                            listOfIndices.push_back(k);
                        }
                    }
                }
                Relation *deleteRelation = joiningRelation;
                joiningRelation = joiningRelation->Project(&listOfIndices);
                delete deleteRelation;
            }
            // 4) Rename the relation to make it union compatible with the scheme in the database (look for the scheme with the same name as the rule)
            Relation* databaseRelation = database->GetMapElement(currentRule->GetHeadPredicate()->GetName());
            joiningRelation->Rename(joiningRelation, &databaseRelation->GetHeader()->GetAttributes());
            // 5) Union with the relation in the database (same name) - modifies database
            bool potentialTupleAdded = false;
            potentialTupleAdded = databaseRelation->Union(joiningRelation);
            if (!tupleAdded) {
                tupleAdded = potentialTupleAdded;
            }
            delete joiningRelation;
            // If new tuples were added restart (use set.insert(myTuple).second which returns a boolean value if the tuple was new), pass through all rules
        }
        numberOfPasses++;
    } while (tupleAdded);

    std::cout << std::endl << "Schemes populated after " << numberOfPasses << " passes through the Rules." << std::endl << std::endl;
    */
    std::cout << std::endl << "Query Evaluation" << std::endl;
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

void Interpreter::EvaluateSCC(std::set<int>* SCC, bool isTrivial) {
    std::cout << "SCC: ";
    OutputSCCNodes(SCC);
    std::cout << std::endl;
    std::vector<Rule*>* ruleVector = program->GetRuleVector();
    bool tupleAdded = false;
    int numberOfPasses = 0;
    do {
        bool currentRoundTupleAdded = false;
        for (auto ruleIt = SCC->begin(); ruleIt != SCC->end(); ruleIt++) {
            // 1) Evaluate the predicates on the right-hand side of the rule (same as queries)
            Rule *currentRule = ruleVector->at((*ruleIt));
            std::cout << currentRule->toString() << std::endl;
            std::vector<Predicate *> predicateList = currentRule->GetPredicateList(); // by reference
            std::vector<Relation *> relationList;
            for (unsigned int j = 0; j < predicateList.size(); j++) {
                relationList.push_back(EvaluatePredicate(*predicateList.at(j)));
            }
            // 2) Join the relations that result
            Relation *joiningRelation = relationList.at(0);
            for (unsigned int j = 1; j < relationList.size(); j++) {
                Relation *deleteRelation = joiningRelation;
                joiningRelation = joiningRelation->Join(relationList.at(j));
                delete deleteRelation;
            }
            // 3) Project the columns that appear in the head predicate (in head predicate order)
            {
                std::vector<int> listOfIndices;
                for (unsigned int j = 0; j < currentRule->GetHeadPredicate()->getList().size(); j++) {
                    for (unsigned int k = 0; k < joiningRelation->GetHeader()->GetAttributes().size(); k++) {
                        if (currentRule->GetHeadPredicate()->getList().at(j)->getParameterValue() ==
                            joiningRelation->GetHeader()->GetAttributes().at(k)) {
                            listOfIndices.push_back(k);
                        }
                    }
                }
                Relation *deleteRelation = joiningRelation;
                joiningRelation = joiningRelation->Project(&listOfIndices);
                delete deleteRelation;
            }
            // 4) Rename the relation to make it union compatible with the scheme in the database (look for the scheme with the same name as the rule)
            Relation *databaseRelation = database->GetMapElement(currentRule->GetHeadPredicate()->GetName());
            joiningRelation->Rename(joiningRelation, &databaseRelation->GetHeader()->GetAttributes());
            // 5) Union with the relation in the database (same name) - modifies database
            bool potentialTupleAdded = false;
            potentialTupleAdded = databaseRelation->Union(joiningRelation);
            if (!currentRoundTupleAdded) {
                currentRoundTupleAdded = potentialTupleAdded;
            }
            delete joiningRelation;
            // If new tuples were added restart (use set.insert(myTuple).second which returns a boolean value if the tuple was new), pass through all rules
        }
        numberOfPasses++;
        if (currentRoundTupleAdded) {
            tupleAdded = true;
        } else {
            tupleAdded = false;
        }
    } while (tupleAdded && !isTrivial);
    std::cout << numberOfPasses << " passes: ";
    OutputSCCNodes(SCC);
    std::cout << std::endl;
}

void Interpreter::OutputSCCNodes(std::set<int> *SCC) {
    bool first = true;
    for (auto it = SCC->begin(); it != SCC->end(); it++) {
        if (!first) {
            std::cout << ",";
        }
        first = false;
        std::cout << "R" << (*it);
    }
}
