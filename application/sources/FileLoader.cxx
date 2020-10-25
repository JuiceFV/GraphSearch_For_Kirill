#include "FileLoader.h"

// the function looks for a specific key. In the case if the
// key is missing the function throw an exception
static inline std::string retrieveKey(const std::string &line)
{
    if (line.find("#type") != std::string::npos) { return "type"; }
    else if (line.find("#vertices") != std::string::npos)
    {
        return "vertices";
    }
    else if (line.find("#edges") != std::string::npos)
    {
        return "edges";
    }
    else if (line.find("#start") != std::string::npos)
    {
        return "start";
    }
    else if (line.find("#goal") != std::string::npos)
    {
        return "goal";
    }
    else
    {
        throw std::invalid_argument(
            "Looked for either \"type\" or \"vertices\" or \"edges\" or \"start\" or \"goal\" after '#' "
            "character, however obtain unknown command" +
            line);
    }
}

static std::unordered_map<std::string, std::vector<std::string>> loadDefaultGraph()
{
    // load the default graph, which one has been taken from here
    // https://csacademy.com/app/graph_editor/
    return {{"type", {"undirected"}},
            {"vertices", defaults::vertices},
            {"edges", defaults::edges},
            {"start", {defaults::start}},
            {"goal", {defaults::goal}}};
}

Loader::Loader()
{
    this->file_path = "";
    this->tokens = loadDefaultGraph();
}

Loader::Loader(const std::string &path)
{
    this->file_path = path;
    tokens = loadGraphFile(this->file_path.string());
}

std::unordered_map<std::string, std::vector<std::string>> Loader::getFileSource() const { return tokens; }

std::unordered_map<std::string, std::vector<std::string>> Loader::loadGraphFile(std::string path) const
{
    std::unordered_map<std::string, std::vector<std::string>> graph_tokens; // the table with tokens and its content
    std::string line;                                                       // each line
    std::string key;                                                        // a token
    std::ifstream graph_file;                                               // stream file
    // check if the file able to throw exceptions
    graph_file.exceptions(std::ifstream::badbit);
    try
    {
        // Open file
        graph_file.open(path);
        if (!graph_file.is_open())
        {
            throw std::ifstream::failure("The path " + fs::absolute(path).string() + " is wrong!");
        }
        // reading file along with processing its content
        while (std::getline(graph_file, line))
        {
            // The graph's code begins from the `!begin` command
            if (line[0] == '!')
            {
                try
                {
                    if (line.find("!begin") != std::string::npos)
                    {
                        // The graph's code ends at the `!end` command
                        while (std::getline(graph_file, line) && line.find("!end") == std::string::npos)
                        {
                            // The graph's rlated data contains within `{}` brackets
                            // The waste is skipping
                            while (line[0] != '{' && !graph_file.eof())
                            {
                                // Each command begins with the `#` sign
                                if (line[0] == '#') key = retrieveKey(line);
                                std::getline(graph_file, line);
                            }
                            graph_tokens[key].push_back(std::string(&line[1], &line[line.find('}')]));
                        }
                    }
                    else
                    {
                        throw std::invalid_argument(
                            "Looked for \"begin\" after '!' character, however begin is missing");
                    }
                }
                catch (std::exception &error)
                {
                    std::cerr << "An error has occured in the " << __FILE__ << " in line " << __LINE__ << "."
                              << std::endl
                              << "Details: " << error.what() << std::endl;
                    exit(-1);
                }
            }
        }
        graph_file.close();
    }
    catch (std::ifstream::failure &error)
    {
        std::cerr << "An error has occured in the " << __FILE__ << " in line " << __LINE__ << "." << std::endl
                  << "Details: " << error.what() << std::endl
                  << "The default Graph has loaded" << std::endl;
        // in the case we miss the file with graph data
        // we load the default one.
        return loadDefaultGraph();
    }
    return (graph_tokens);
}

Loader::~Loader() {}
