#include "Graph.h"

void Graph::fillFromLoadedFile()
{
    Loader graph_code;
    std::unordered_map<std::string, std::vector<std::string>> graph_tokens = graph_code.getFileSource();
    try
    {
        if (graph_tokens["type"][0] == "undirected")
            this->type = GRAPH_TYPE::UNDIRECTED;
        else if (graph_tokens["type"][0] == "directed")
            this->type = GRAPH_TYPE::DIRECTED;
        else
        {
            throw std::invalid_argument(
                "Looked for graph's type either \"undirected\" or \"directed\", however obtain unknown type!");
        }
        this->start = {graph_tokens["start"][0]};
        this->goal = {graph_tokens["goal"][0]};
        if (this->type == GRAPH_TYPE::UNDIRECTED) { 
            for (auto &vertex : graph_tokens["vertices"])
            {
                this->connections[{vertex}] = nullptr;
            }
        }
        else
        {
            //fill as directed
        }
    }
    catch (std::exception &error)
    {
        std::cerr << "An error has occured in the " << __FILE__ << " in line " << __LINE__ << "." << std::endl
                  << "Details: " << error.what() << std::endl;
        exit(-1);
    }
}
