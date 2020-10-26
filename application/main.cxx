#include "Solver.h"

int main()
{
    setlocale(LC_ALL, "rus");
    
    Solver g1_dir;
    if (g1_dir.breadthFirstSearch())
    {
        g1_dir.printPath();
        std::cout << std::endl << "Number of steps: " << g1_dir.getSteps() << std::endl;
    }else
    {
        std::cerr << "I'm not able to detect a way to the " << g1_dir.getGoal() << " from the " << g1_dir.getStart() << std::endl; 
    }

    return (0);
}
