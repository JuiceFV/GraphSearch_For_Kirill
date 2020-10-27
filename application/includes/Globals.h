#pragma once

#include <string>
#include <vector>

// я использовал `enum class`, потому что имена enum-a €вл€ютс€ локальными дл€ enum,
// и их значени€ не конвертируютс€ не€вно в другие типы,
// в то врем€ как `enum` так делает(например, в int)

// тип графа
enum class GRAPH_TYPE
{
    UNDIRECTED,
    DIRECTED
};

namespace defaults
{
    // данные дл€ дефолтного графа
    constexpr GRAPH_TYPE graph_type = GRAPH_TYPE::UNDIRECTED;

    const std::vector<std::string> vertices({"0", "1", "2", "3", "4", "5"});
    const std::vector<std::string> edges({"0->2", "0->4", "0->5", "1->4", "1->5", "2->3", "2<->4", "4->5"});
    const std::string start = "0";
    const std::string goal = "3";

} // namespace defaults
