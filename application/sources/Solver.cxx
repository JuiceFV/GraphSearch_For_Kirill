#include "Solver.h"

Solver::Solver(const Graph &graph)
{
    this->exploring_graph = graph;
    this->path = {};
    this->steps = 0;
    this->start = exploring_graph.getStart();
    this->goal = exploring_graph.getGoal();
    // изначально, нод который нужно проверить - стартовый
    this->prepared_nodes.push_back(this->start); 
}

Solver::Solver()
{
    this->path = {};
    this->steps = 0;
    this->start = exploring_graph.getStart();
    this->goal = exploring_graph.getGoal();
    // изначально, нод который нужно проверить - стартовый
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

// поиск в ширину
bool Solver::breadthFirstSearch()
{
    // в bfs мы считываем по уровням
    // т.к. нужно сделать с помощью рекурсии
    // тогда во время проверки каждого нода в ширину
    // мы записываем их детей в prepared_node
    std::list<Node> new_prep_nodes;
    // проверяем каждый нод
    for (auto &node : this->prepared_nodes)
    {
        // если нод еще не проверен (в комплексных граффах)
        if (std::find(this->explored_nodes.begin(), this->explored_nodes.end(), node) == this->explored_nodes.end())
        {
            // записываем в путь и прибавляем кол-во шагов
            this->path.push_back(node);
            this->steps++;
            // проверяем это наша цель или нет
            if (node == this->goal) return true;
            // если нет - бросаем нод в исследованные
            this->explored_nodes.push_back(node);

            // получаем детей данного нода
            auto list = this->exploring_graph[node];
            // для комплексных многосвзных графов
            // получаем уникальные ноды (если 1 соединена с 0 и 4, в то время как 4 так же 
            // соединена с 0, тогда 0 повторится два раза.)
            list.unique();
            // так же текщий нод может быть соединён с уже иследованными
            // удаляем эти (исследованные) ноды
            list.remove_if([this](Node n) {
                return std::find(this->explored_nodes.begin(), this->explored_nodes.end(), n) ==
                               this->explored_nodes.end()
                           ? false
                           : true;
            });
            // т.к. мы ищем в ширину, мы постояно добавляем детей каждого нода, n-ого уровня 
            new_prep_nodes.insert(new_prep_nodes.end(), list.begin(), list.end());
        }
    }
    // если дети есть - устанавливаем их для проверки
    // и запускаем рекурсию (мы вне цикла for) мы проверяем по уровням
    // с каждой иттерацией рекурсии мы идем всё глубже и глубже, проверя в ширину
    if (!new_prep_nodes.empty())
    {
        this->prepared_nodes = new_prep_nodes;
        return breadthFirstSearch();
    }
    return false;
}

bool Solver::depthFirstSearch()
{
    // проверяем каждый нод
    for (auto &node : this->prepared_nodes)
    {
        // если нод еще не проверен (в комплексных граффах)
        if (std::find(this->explored_nodes.begin(), this->explored_nodes.end(), node) == this->explored_nodes.end())
        {
            // записываем в путь и прибавляем кол-во шагов
            this->path.push_back(node);
            this->steps++;
            // проверяем это наша цель или нет
            if (node == this->goal) return true;
            // если нет - бросаем нод в исследованные
            this->explored_nodes.push_back(node);

            // получаем детей данного нода
            auto list = this->exploring_graph[node];
            // т.к. мы ищем в глубину то дубликатами могут быть уже иследованные
            // удаляем их (иследованные).
            list.remove_if([this](Node n) {
                return std::find(this->explored_nodes.begin(), this->explored_nodes.end(), n) ==
                               this->explored_nodes.end()
                           ? false
                           : true;
            });

            // если дети нашлись
            // мы запускаем рекурсию (на данный момент мы в цикле, т.е. рекурсия запускается для каждого нода)
            if (!list.empty())
            {
                // в dfs важен порядок поиска, поэтому вставляем детей, которые нужно иследовть
                // в начало.
                this->prepared_nodes.insert(this->prepared_nodes.begin(), list.begin(), list.end());
                // удаляем текущий нод, который был добавлен в предыдущей иттерации рекурсии
                this->prepared_nodes.remove(node);
                // унифицируем
                this->prepared_nodes.unique();
                return depthFirstSearch();
            }
        }
    }
    return false;
}
