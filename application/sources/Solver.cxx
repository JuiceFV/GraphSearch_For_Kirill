#include "Solver.h"

Solver_Directed::Solver_Directed(Graph &graph)
{
    this->exploring_graph = graph;
    this->path = {};
    this->steps = 0;
    this->start = exploring_graph.getStart();
    this->goal = exploring_graph.getGoal();
    this->current_node = this->start;
    this->bfs_for_check.push_back(this->current_node);
}

Solver_Undirected::Solver_Undirected()
{
    this->path = {};
    this->steps = 0;
    this->start = exploring_graph.getStart();
    this->goal = exploring_graph.getGoal();
    this->current_node = this->start;
    this->bfs_for_check.push_back(this->current_node);
}

Solver_Undirected::Solver_Undirected(Graph &graph)
{
    this->exploring_graph = graph;
    this->path = {};
    this->steps = 0;
    this->start = exploring_graph.getStart();
    this->goal = exploring_graph.getGoal();
    this->current_node = this->start;
    this->bfs_for_check.push_back(this->current_node);
}

void Solver::setStart(const Node &n) { this->start = n; }
void Solver::setGoal(const Node &n) { this->goal = n; }
void Solver::printPath() const
{
    std::cout << "The path is:" << std::endl;
    for (int i = 0; i < this->path.size() - 1; i++) std::cout << this->path[i] << " --> ";
    std::cout << this->path.back();
}
unsigned long long Solver::getSteps() const { return this->steps; }
bool Solver_Undirected::breadthFirstSearch()
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
                return std::find(this->explored_nodes.begin(), this->explored_nodes.end(), n) == this->explored_nodes.end()
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
}
