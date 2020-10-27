#pragma once

#include "Common.hpp"
#include "Graph.h"
#include <iostream>

class Solver
{
  public:
    // дефолтный конструктор, подгружает дефолтный граф
    Solver();
    // обычный конструктор, подгружает граф из файла
    Solver(const Graph &graph);
    bool breadthFirstSearch();           // поиск в ширину
    bool depthFirstSearch();             // поиск в глубину
    void setStart(const Node &n);        // установить начальный нод
    void setGoal(const Node &n);         // установить цель
    void printPath() const;              // напечатать путь
    Node getStart() const;               // получить стартовый нод
    Node getGoal() const;                // получить цель
    unsigned long long getSteps() const; // получить кол-во пройденных шагов
    ~Solver(){};

  private:
    unsigned long long steps;       // кол-во пройденных шагов
    Graph exploring_graph;          // исследуемый граф
    std::vector<Node> path;         // пройденный путь
    Node start;                     // стартовый нод
    Node goal;                      // цель
    std::list<Node> explored_nodes; // пройденные ноды, которые не €вл€ютс€ целью
    std::list<Node> prepared_nodes; // ноды которые будут проверены на следующем шаге
};
