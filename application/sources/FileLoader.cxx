#include "FileLoader.h"

// ��� ������� ���� ����������� ����� � �����
// ��� ������ ����������, ��� ������� ������ ����
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
    // ���������� ��������� ����, ������� ��� ���� �� ����
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

// ���������� ������ ��� ����� � ���� ���������
std::unordered_map<std::string, std::vector<std::string>> Loader::getFileSource() const { return tokens; }

std::unordered_map<std::string, std::vector<std::string>> Loader::loadGraphFile(std::string path) const
{
    // ��� �������, ��� ������ - ����� (#token), � �������� - �������� ������ (������ �����)
    std::unordered_map<std::string, std::vector<std::string>> graph_tokens;
    std::string line;         // ������ �����
    std::string key;          // �����
    std::ifstream graph_file; // ����������� ����
    // ����� �� ���� ������ ����������
    graph_file.exceptions(std::ifstream::badbit);
    try
    {
        // ��������� ����
        graph_file.open(path);
        if (!graph_file.is_open())
        {
            // ���� �� �������� ������ ���������� � ������ ���� �����
            throw std::ifstream::failure("The path " + fs::absolute(path).string() + " is wrong!");
        }
        // ������ ����
        while (std::getline(graph_file, line))
        {
            // "���������" ������ ���������� �� ����� !begin
            // �������� ����� �������, ���� ��������� �� ������
            // ���� ���-�� ��� ������� `!sbvsk`
            if (line[0] == '!')
            {
                try
                {
                    // ���� ���� ! ��� begin - ����� ���� ������ � ��������� ������
                    if (line.find("!begin") != std::string::npos)
                    {
                        // ���� �� ����� �����/����
                        while (std::getline(graph_file, line) && line.find("!end") == std::string::npos)
                        {
                            // ���� �� ����� ������ ��� ����� ��������� ����
                            while (line[0] != '{' && !graph_file.eof())
                            {
                                // ���� ����� ���������� � # - ��� �����
                                // �� ���� ����� ������ ������ ���� ������ ��� ������
                                // ��� ��� ����� �� ����� '{'.
                                if (line[0] == '#') key = retrieveKey(line);
                                std::getline(graph_file, line);
                            }
                            // ���� '{' ���� �������, ������ ��� ������ ��� �������� ������
                            // ������ �� (����� � ��� ������) � ��������
                            graph_tokens[key].push_back(std::string(&line[1], &line[line.find('}')]));
                        }
                    }
                    else
                    {
                        // ���� ����� �����. ����� ��� �� begin  - ������ ����������
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
        // ��� �������, ��� ���� ��� ������� ������� - �� ���������� ��������� ������
        // ������� ���������� � Globals.h
        return loadDefaultGraph();
    }
    return (graph_tokens);
}

Loader::~Loader() {}
