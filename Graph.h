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
    // TODO (1) Create Graph class
    /* Adjacency list implementation
     * Graph stores all of the edges/adjacencies in a map<int nodeIdentifier, set<int> neighbors>
     * Each node should also have a boolean visited flag map<int nodeIdentifier, boolean visited>
     * dfs, dfsForest, (reversePostOrder shouldn't need to be done if use stack when creating postorder), push opposite edge on reverse graph instead of creating reverse graph
     * Two dfsForest functions (one for post-order, one for SCCs)
     * toString() should have each rule and its dependencies (even if rule doesn't have dependencies)
     * Note the identifier should be an int so that we print out the identifiers in the correct order
     */
};


#endif //PROJECT3_GRAPH_H
