#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "Common.hpp"
#include "FileLoader.h"
#include "Globals.h"
#include "WordSimilarity.h"

class Graph
{
  public:
    // Дефолтный конструктор, подгружает дефолтный граф
    Graph();
    // обычный еонструктор, подгружает граф из файла
    Graph(const std::string path);
    // конструктор копирования - копирует o_0
    Graph(const Graph &graph);
    // возвращает точку отправления
    Node getStart() const;
    // возвращает точку назначения
    Node getGoal() const;
    // перегруженный оператор - возвращает детей определённого нода graph[node] = {child0, child1, ..., childn}
    std::list<Node> &operator[](const Node &index);

    // перегрузка вывода, не буду её пояснять, она не нужна для работы логики
    friend std::ostream &operator<<(std::ostream &out, const Graph &obj)
    {

        out << "\nType:" << std::endl
            << "\t" << (obj.type == GRAPH_TYPE::DIRECTED ? "directed" : "undirected") << std::endl
            << "Vertices:" << std::endl;
        for (auto &vertex : obj.connections) out << "\t" << vertex.first << std::endl;
        out << "Edges:" << std::endl;
        std::vector<Node> closed_nodes;
        if (obj.type == GRAPH_TYPE::DIRECTED)
        {
            for (auto &vertex : obj.connections)
            {
                closed_nodes.push_back(vertex.first);
                std::string edge_type;
                for (auto &node : vertex.second)
                {
                    if (std::find(closed_nodes.begin(), closed_nodes.end(), node) == closed_nodes.end())
                    {
                        auto searh_list = obj.connections.at(node);
                        edge_type = std::find(searh_list.begin(), searh_list.end(), vertex.first) != searh_list.end()
                                        ? "<->"
                                        : "->";
                        out << "\t" << vertex.first << edge_type << node << std::endl;
                    }
                }
            }
        }
        else
        {
            for (auto &vertex : obj.connections)
            {
                closed_nodes.push_back(vertex.first);
                for (auto &node : vertex.second)
                {
                    if (std::find(closed_nodes.begin(), closed_nodes.end(), node) == closed_nodes.end())
                    {
                        out << "\t" << vertex.first << "--" << node << std::endl;
                    }
                }
            }
        }
        out << "Start:" << std::endl
            << "\t" << obj.start << std::endl
            << "Goal:" << std::endl
            << "\t" << obj.goal << std::endl;
        return out;
    }

  private:
    GRAPH_TYPE type;                                       // тип графа
    Node start;                                            // начальный нод
    Node goal;                                             // цель
    std::unordered_map<Node, std::list<Node>> connections; // соединения нодов между собой
    void fillFromLoadedFile(const Loader &graph_code); // заполнить граф и подгруженного файла
};
