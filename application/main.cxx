#include "Solver.h"

int main()
{
    setlocale(LC_ALL, "rus");

    Solver g1_dir(Graph("graphs/undirectedGraph1.grph"));
    std::cout << "Choose an algo:\n";
    std::cout << "[1] dfs\n"
              << "[2] bfs\n";

    std::string input;
    std::getline(std::cin, input);

    while (input != "1" && input != "2")
    {
        std::cout << "Invalid!\n\n";

        std::cout << "Choose an algo:\n";
        std::cout << "[1] dfs\n"
                  << "[2] bfs\n";
        std::getline(std::cin, input);
    }

    if (input == "1")
    {
        // plug in to a function
        if (g1_dir.depthFirstSearch())
        {
            g1_dir.printPath();
            std::cout << std::endl << "Number of steps: " << g1_dir.getSteps() << std::endl;
        }
        else
        {
            std::cerr << "I'm not able to detect a way to the " << g1_dir.getGoal() << " from the " << g1_dir.getStart()
                      << std::endl;
        }
    }
    else if (input == "2")
    {
        // plug in to a function
        if (g1_dir.breadthFirstSearch())
        {
            g1_dir.printPath();
            std::cout << std::endl << "Number of steps: " << g1_dir.getSteps() << std::endl;
        }
        else
        {
            std::cerr << "I'm not able to detect a way to the " << g1_dir.getGoal() << " from the " << g1_dir.getStart()
                      << std::endl;
        }
    }
    

    return (0);
}
