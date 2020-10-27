#pragma once

#include <omp.h> // для специализации хэширования

#include <cmath>      // мне нужна была математика для логарифма
#include <functional> // для файловой системы
#include <iostream>
#include <list>
#include <unordered_map> // hash-map
#include <utility>

// структура нода
// каждый нод репрезентуется посредством своего уникального имени
typedef struct Node
{
    std::string name; // имя нода

    // для удобства использования я перегрузил операторы сравенния
    bool operator==(const Node &n) const { return (name == n.name); }
    bool operator!=(const Node &n) const { return (name != n.name); }

    // и оператор побитового сдвига для удобства вывода
    friend std::ostream &operator<<(std::ostream &out, const Node &n)
    {
        out << n.name;
        return out;
    }
} Node;

namespace std
{
    // в связи с тем что, базовый stl-ный hash-map не имет возможности
    // хэширования произвольных (не STL) объектов, пришлось специализировать
    // её под нашу структуру нода.
    // Здесь можно найти всю инфу: https://en.cppreference.com/w/cpp/utility/hash
    template <> struct hash<Node>
    {
        // если кратко при вызове конструктора
        // из объекта переданного как параметр
        // извлекается уникальный ключ и возвращается
        // в виде уникального индетификатора данного объекта
        std::size_t operator()(const Node &n) const
        {
            std::size_t const h1 = std::hash<std::string>{}(n.name);
            return h1;
        }
    };

    // т.к. в stl нету резделения по делимитру, пришлось делать самому
    std::vector<std::string> strSplit(std::string s, const std::vector<std::string> &delim);
} // namespace std
