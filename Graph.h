//
// Created by Jesse Clark on 6/4/21.
//

#ifndef PROJECT3_GRAPH_H
#define PROJECT3_GRAPH_H
#include <map>
#include <set>
#include <stack>
#include <vector>
#include "Rule.h"
#include <sstream>

class Graph {
private:
    std::map<int, std::set<int>> edges;
    std::map<int, bool> nodeVisited;
public:
    Graph(std::vector<Rule*>* ruleVector, bool reversed);
    void DepthFirstSearch(int vertex, std::stack<int>* postOrder);
    void DepthFirstSearch(int vertex, std::set<int>* newTree);
    std::stack<int>* DepthFirstSearchForest();
    std::vector<std::set<int>*>* DepthFirstSearchForestSCC(std::stack<int>* postOrder);
    std::string toString();
    bool IsTrivial(int nodeIdentifier);
};


#endif //PROJECT3_GRAPH_H
