#include "Solver.h"

int main()
{
    setlocale(LC_ALL, "rus");
    Graph g("graphs/undirectedGraph1.grph");
    Solver_Directed g1_dir(g);
    if (g1_dir.breadthFirstSearch())
    {
        g1_dir.printPath();
        std::cout << std::endl << "Number of steps: " << g1_dir.getSteps() << std::endl;
    }

    return (0);
}
