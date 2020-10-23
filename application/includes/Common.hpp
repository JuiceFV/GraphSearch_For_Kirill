#pragma once
#include "Graph.h"
#include <omp.h>

#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <queue>
#include <unordered_map>
#include <utility>

typedef struct Node
{
    std::string name;
    bool operator==(const Node &n) const { return (name == n.name); }

    bool operator!=(const Node &n) const { return (name != n.name); }
    friend std::ostream &operator<<(std::ostream &out, const Node &n)
    {
        out << n.name;
        return out;
    }
} Node;

namespace std
{
    template <> struct hash<Node>
    {
        std::size_t operator()(const Node &n) const
        {
            std::size_t const h1 = std::hash<std::string>{}(n.name);
            std::size_t const h2 = std::hash<std::string>{}(n.name);
            return (h1 ^ (h2 << 1));
        }
    };

    std::vector<std::string> strSplit(const std::string &s, char delim)
    {
        std::vector<std::string> result;
        std::stringstream ss(s);
        std::string item;

        while (getline(ss, item, delim)) { result.push_back(item); }

        return result;
    }
} // namespace std
