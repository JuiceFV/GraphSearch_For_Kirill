#pragma once

#include "Common.hpp"
#include "Graph.h"
#include <iostream>

class Solver
{
  public:
    Solver();
    Solver(const Graph &graph);
    bool breadthFirstSearch();
    bool depthFirstSearch();
    void setStart(const Node &n);
    void setGoal(const Node &n);
    void printPath() const;
    Node getStart() const;
    Node getGoal() const;
    unsigned long long getSteps() const;
    ~Solver() {};

  private:
    unsigned long long steps;
    Graph exploring_graph;
    std::vector<Node> path;
    Node start;
    Node goal;
    Node current_node;
    std::list<Node> explored_nodes;
    std::list<Node> prepared_nodes;
};

