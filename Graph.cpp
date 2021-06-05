//
// Created by Jesse Clark on 6/4/21.
//

#include "Graph.h"

Graph::Graph(std::vector<Rule *> *ruleVector, bool reversed) {
    for (unsigned int i = 0; i < ruleVector->size(); i++) {
        Rule *currentRule = ruleVector->at(i);
        edges.insert(std::pair<int, std::set<int>>(i, std::set<int>()));
        nodeVisited.insert(std::pair<int, bool>(i, false));
    }
    for (unsigned int i = 0; i < ruleVector->size(); i++) {
        Rule* currentRule = ruleVector->at(i);
        for (unsigned int j = 0; j < ruleVector->size(); j++) {
            Rule* compareRule = ruleVector->at(j);
            std::vector<Predicate*> predicateList = compareRule->GetPredicateList();
            for (unsigned int k = 0; k < predicateList.size(); k++) {
                if (predicateList.at(k)->GetName() == currentRule->GetHeadPredicate()->GetName()) {
                    if (!reversed) {
                        edges.at(j).insert(i);
                    } else {
                        edges.at(i).insert(j);
                    }
                }
            }
        }
    }

}

std::string Graph::toString() {
    std::stringstream ss;
    bool firstRule = true;
    for (auto it = edges.begin(); it != edges.end(); it++) {
        if (!firstRule) {
            ss << '\n';
        }
        firstRule = false;
        ss << "R" << (*it).first << ":";
        bool first = true;
        for (auto neighborIt = (*it).second.begin(); neighborIt != (*it).second.end(); neighborIt++) {
            if (!first) {
                ss << ",";
            }
            first = false;
            ss << "R" << (*neighborIt);
        }
    }

    return ss.str();
}

void Graph::DepthFirstSearch() {

}

void Graph::DepthFirstSearchHelper() {

}
