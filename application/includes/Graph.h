#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "FileLoader.h"
#include "Globals.h"
#include "Common.hpp"

class Graph
{
  public:
    Graph();
    Graph(const std::string path);
    Graph(const Graph& graph);
  private:
    GRAPH_TYPE type;
    Node start;
    Node goal;
    std::unordered_map<Node, std::list<Node>*> connections;
    void fillFromLoadedFile();
    
    
      
};
