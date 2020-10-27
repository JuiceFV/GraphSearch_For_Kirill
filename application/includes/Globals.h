#pragma once

#include <string>
#include <vector>

// � ����������� `enum class`, ������ ��� ����� enum-a �������� ���������� ��� enum,
// � �� �������� �� �������������� ������ � ������ ����,
// � �� ����� ��� `enum` ��� ������(��������, � int)

// ��� �����
enum class GRAPH_TYPE
{
    UNDIRECTED,
    DIRECTED
};

namespace defaults
{
    // ������ ��� ���������� �����
    constexpr GRAPH_TYPE graph_type = GRAPH_TYPE::UNDIRECTED;

    const std::vector<std::string> vertices({"0", "1", "2", "3", "4", "5"});
    const std::vector<std::string> edges({"0->2", "0->4", "0->5", "1->4", "1->5", "2->3", "2<->4", "4->5"});
    const std::string start = "0";
    const std::string goal = "3";

} // namespace defaults
