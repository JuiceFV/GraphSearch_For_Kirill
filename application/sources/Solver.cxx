#include "Solver.h"

Solver::Solver(const Graph &graph)
{
    this->exploring_graph = graph;
    this->path = {};
    this->steps = 0;
    this->start = exploring_graph.getStart();
    this->goal = exploring_graph.getGoal();
    // ����������, ��� ������� ����� ��������� - ���������
    this->prepared_nodes.push_back(this->start); 
}

Solver::Solver()
{
    this->path = {};
    this->steps = 0;
    this->start = exploring_graph.getStart();
    this->goal = exploring_graph.getGoal();
    // ����������, ��� ������� ����� ��������� - ���������
    this->prepared_nodes.push_back(this->start);
}

void Solver::setStart(const Node &n) { this->start = n; }
void Solver::setGoal(const Node &n) { this->goal = n; }
Node Solver::getStart() const { return this->goal; }
Node Solver::getGoal() const { return this->start; }

void Solver::printPath() const
{
    std::cout << "The path is:" << std::endl;
    for (int i = 0; i < this->path.size() - 1; i++) std::cout << this->path[i] << " --> ";
    std::cout << this->path.back();
}

unsigned long long Solver::getSteps() const { return this->steps; }

// ����� � ������
bool Solver::breadthFirstSearch()
{
    // � bfs �� ��������� �� �������
    // �.�. ����� ������� � ������� ��������
    // ����� �� ����� �������� ������� ���� � ������
    // �� ���������� �� ����� � prepared_node
    std::list<Node> new_prep_nodes;
    // ��������� ������ ���
    for (auto &node : this->prepared_nodes)
    {
        // ���� ��� ��� �� �������� (� ����������� �������)
        if (std::find(this->explored_nodes.begin(), this->explored_nodes.end(), node) == this->explored_nodes.end())
        {
            // ���������� � ���� � ���������� ���-�� �����
            this->path.push_back(node);
            this->steps++;
            // ��������� ��� ���� ���� ��� ���
            if (node == this->goal) return true;
            // ���� ��� - ������� ��� � �������������
            this->explored_nodes.push_back(node);

            // �������� ����� ������� ����
            auto list = this->exploring_graph[node];
            // ��� ����������� ����������� ������
            // �������� ���������� ���� (���� 1 ��������� � 0 � 4, � �� ����� ��� 4 ��� �� 
            // ��������� � 0, ����� 0 ���������� ��� ����.)
            list.unique();
            // ��� �� ������ ��� ����� ���� ������� � ��� �������������
            // ������� ��� (�������������) ����
            list.remove_if([this](Node n) {
                return std::find(this->explored_nodes.begin(), this->explored_nodes.end(), n) ==
                               this->explored_nodes.end()
                           ? false
                           : true;
            });
            // �.�. �� ���� � ������, �� �������� ��������� ����� ������� ����, n-��� ������ 
            new_prep_nodes.insert(new_prep_nodes.end(), list.begin(), list.end());
        }
    }
    // ���� ���� ���� - ������������� �� ��� ��������
    // � ��������� �������� (�� ��� ����� for) �� ��������� �� �������
    // � ������ ���������� �������� �� ���� �� ������ � ������, ������� � ������
    if (!new_prep_nodes.empty())
    {
        this->prepared_nodes = new_prep_nodes;
        return breadthFirstSearch();
    }
    return false;
}

bool Solver::depthFirstSearch()
{
    // ��������� ������ ���
    for (auto &node : this->prepared_nodes)
    {
        // ���� ��� ��� �� �������� (� ����������� �������)
        if (std::find(this->explored_nodes.begin(), this->explored_nodes.end(), node) == this->explored_nodes.end())
        {
            // ���������� � ���� � ���������� ���-�� �����
            this->path.push_back(node);
            this->steps++;
            // ��������� ��� ���� ���� ��� ���
            if (node == this->goal) return true;
            // ���� ��� - ������� ��� � �������������
            this->explored_nodes.push_back(node);

            // �������� ����� ������� ����
            auto list = this->exploring_graph[node];
            // �.�. �� ���� � ������� �� ����������� ����� ���� ��� ������������
            // ������� �� (������������).
            list.remove_if([this](Node n) {
                return std::find(this->explored_nodes.begin(), this->explored_nodes.end(), n) ==
                               this->explored_nodes.end()
                           ? false
                           : true;
            });

            // ���� ���� �������
            // �� ��������� �������� (�� ������ ������ �� � �����, �.�. �������� ����������� ��� ������� ����)
            if (!list.empty())
            {
                // � dfs ����� ������� ������, ������� ��������� �����, ������� ����� ���������
                // � ������.
                this->prepared_nodes.insert(this->prepared_nodes.begin(), list.begin(), list.end());
                // ������� ������� ���, ������� ��� �������� � ���������� ��������� ��������
                this->prepared_nodes.remove(node);
                // �����������
                this->prepared_nodes.unique();
                return depthFirstSearch();
            }
        }
    }
    return false;
}
