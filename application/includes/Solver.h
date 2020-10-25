#pragma once

#include "Common.hpp"
#include "Graph.h"
#include <iostream>

class Solver
{
  public:
    virtual bool breadthFirstSearch() = 0;
    virtual bool depthFirstSearch() = 0;
    void setStart(const Node &n);
    void setGoal(const Node &n);
    void printPath() const;
    unsigned long long getSteps() const;
    ~Solver() {};

  protected:
    unsigned long long steps;
    Graph exploring_graph;
    std::vector<Node> path;
    Node start;
    Node goal;
    Node current_node;
    std::list<Node> explored_nodes;
    std::list<Node> bfs_for_check;
};

class Solver_Directed : public Solver
{
  public:
    Solver_Directed(Graph &graph);
    bool breadthFirstSearch()
    {
        std::list<Node> new_for_check;
        for (auto &node : this->bfs_for_check)
        {
            if (std::find(this->explored_nodes.begin(), this->explored_nodes.end(), node) == this->explored_nodes.end())
            {
                this->path.push_back(node);
                this->steps++;
                if (node == this->goal) return true;
                this->explored_nodes.push_back(node);

                auto list = this->exploring_graph[node];
                list.unique();
                list.remove_if([this](Node n) {
                    return std::find(this->explored_nodes.begin(), this->explored_nodes.end(), n) ==
                                   this->explored_nodes.end()
                               ? false
                               : true;
                });
                new_for_check.insert(new_for_check.end(), list.begin(), list.end());
            }
        }
        if (!new_for_check.empty())
        {
            this->bfs_for_check = new_for_check;
            return breadthFirstSearch();
        }
        return false;
    };
    bool depthFirstSearch() { return false; };
};

class Solver_Undirected : public Solver
{
  public:
    Solver_Undirected();
    Solver_Undirected(Graph &graph);
    bool breadthFirstSearch();
    bool depthFirstSearch() { return false; };
};
