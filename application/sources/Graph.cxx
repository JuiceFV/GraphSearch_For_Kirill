#include "Graph.h"

static void wordAutocorrection(const std::vector<std::string> &possible_words, std::string &word, uint8_t n = 2)
{
    std::cerr << "You've admitted a mistake in typing '" << word << "'" << std::endl;
    if (word.size() == 1)
    {
        std::cerr << "Missing the vertex with name " << word << std::endl;
        exit(-1);
    }
    else if (word.size() == 2)
    {
        n = 1;
    }
    word = smlrt::findeSimilarWord(possible_words, word, n);
    std::cerr << "I assume you've implied the '" << word << "', thus the wrong one will be replaced" << std::endl;
}

Graph::Graph()
{
    Loader default_graph;
    fillFromLoadedFile(default_graph);
}
Graph::Graph(const std::string path)
{
    Loader graph_file(path);
    fillFromLoadedFile(graph_file);
}
Graph::Graph(const Graph &graph)
{
    this->type = graph.type;
    this->start = graph.start;
    this->goal = graph.goal;
    this->connections = graph.connections;
}

Node Graph::getStart() const { return this->start; }
Node Graph::getGoal() const { return this->goal; }

std::list<Node> &Graph::operator[](const Node &index)
{
    if (this->connections.find(index) != this->connections.end()) return this->connections[index];
    else
    {
        std::cerr << "Wrong index" << std::endl;
        exit(-1);
    }
}

void Graph::fillFromLoadedFile(const Loader &graph_code)
{
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

        for (auto &vertex : graph_tokens["vertices"]) { this->connections[{vertex}] = {}; }

        if (this->connections.find({graph_tokens["start"][0]}) == this->connections.end())
            wordAutocorrection(graph_tokens["vertices"], graph_tokens["start"][0]);
        if (this->connections.find({graph_tokens["goal"][0]}) == this->connections.end())
            wordAutocorrection(graph_tokens["vertices"], graph_tokens["goal"][0]);

        this->start = {graph_tokens["start"][0]};
        this->goal = {graph_tokens["goal"][0]};

        if (this->type == GRAPH_TYPE::UNDIRECTED)
        {
            for (auto &edge : graph_tokens["edges"])
            {
                // DO NOT CHANGE THE ORDER OF DELIMETERS
                // The problem is that the '->' is part of the '<->'
                // but the strSplit returns vector of words separated
                // by the first occured delimeter.
                std::vector<std::string> destinations = std::strSplit(edge, {"<->", "->"});
                if (destinations.empty())
                {
                    throw std::invalid_argument(
                        "Looked for either '->' or '<->' between the destination's points, however it's missing!");
                }
                else
                {
                    if (this->connections.find({destinations[0]}) == this->connections.end())
                        wordAutocorrection(graph_tokens["vertices"], destinations[0]);
                    if (this->connections.find({destinations[1]}) == this->connections.end())
                        wordAutocorrection(graph_tokens["vertices"], destinations[1]);

                    auto search_list = this->connections[{destinations[1]}];
                    if (std::find(search_list.begin(), search_list.end(), Node{destinations[0]}) == search_list.end())
                    {
                        this->connections[{destinations[0]}].push_back({destinations[1]});
                        this->connections[{destinations[1]}].push_back({destinations[0]});
                    }
                }
            }
        }
        else
        {
            for (auto &edge : graph_tokens["edges"])
            {
                // DO NOT CHANGE THE ORDER OF DELIMETERS
                // The problem is that the '->' is part of the '<->'
                // but the strSplit returns vector of words separated
                // by the first occured delimeter.
                std::vector<std::string> destinations = std::strSplit(edge, {"<->", "->"});
                if (destinations.empty())
                {
                    throw std::invalid_argument(
                        "Looked for either '->' or '<->' between the destination's points, however it's missing!");
                }
                else
                {
                    if (this->connections.find({destinations[0]}) == this->connections.end())
                        wordAutocorrection(graph_tokens["vertices"], destinations[0]);
                    if (this->connections.find({destinations[1]}) == this->connections.end())
                        wordAutocorrection(graph_tokens["vertices"], destinations[1]);

                    this->connections[{destinations[0]}].push_back({destinations[1]});
                    if (edge.find("<->") != std::string::npos)
                        this->connections[{destinations[1]}].push_back({destinations[0]});
                }
            }
        }
    }
    catch (std::exception &error)
    {
        std::cerr << "An error has occured in the " << __FILE__ << " in line " << __LINE__ << "." << std::endl
                  << "Details: " << error.what() << std::endl;
        exit(-1);
    }
}
