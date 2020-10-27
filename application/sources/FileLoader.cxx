#include "FileLoader.h"

// эта функция ищет определённый токен в линии
// или кидает исключение, что данного токена нету
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
    // подгружаем дефолтный граф, который был взят от сюда
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

// возвращаем данные для графа в виде хэштэйбла
std::unordered_map<std::string, std::vector<std::string>> Loader::getFileSource() const { return tokens; }

std::unordered_map<std::string, std::vector<std::string>> Loader::loadGraphFile(std::string path) const
{
    // хэш таблица, где индекс - токен (#token), а значения - значения токена (данные графа)
    std::unordered_map<std::string, std::vector<std::string>> graph_tokens;
    std::string line;         // каждая линия
    std::string key;          // токен
    std::ifstream graph_file; // считываемый файл
    // может ли файл кидать исключения
    graph_file.exceptions(std::ifstream::badbit);
    try
    {
        // Открываем файл
        graph_file.open(path);
        if (!graph_file.is_open())
        {
            // Если не открылся кидаем исключение с полным путём файла
            throw std::ifstream::failure("The path " + fs::absolute(path).string() + " is wrong!");
        }
        // читаем файл
        while (std::getline(graph_file, line))
        {
            // "программа" граффа начинается со слова !begin
            // разделил таким образом, дабы проверить на дурака
            // Если кто-то так напишет `!sbvsk`
            if (line[0] == '!')
            {
                try
                {
                    // если поле ! идёт begin - тогда ищем токены и извлекаем данные
                    if (line.find("!begin") != std::string::npos)
                    {
                        // пока не конец файла/кода
                        while (std::getline(graph_file, line) && line.find("!end") == std::string::npos)
                        {
                            // пока не нашли данные для графа считываем файл
                            while (line[0] != '{' && !graph_file.eof())
                            {
                                // если линия начинается с # - это токен
                                // по идее после токена должны идти данные для токена
                                // так что скоро мы найдём '{'.
                                if (line[0] == '#') key = retrieveKey(line);
                                std::getline(graph_file, line);
                            }
                            // если '{' была найдена, значит это данные для текущего токена
                            // кидаем их (токен и его данные) в хэштэйбл
                            graph_tokens[key].push_back(std::string(&line[1], &line[line.find('}')]));
                        }
                    }
                    else
                    {
                        // если после воскл. знака идёт не begin  - кидаем исключение
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
        // при условии, что путь был передан неверно - мы подгружаем дефолтный конфиг
        // который расположен в Globals.h
        return loadDefaultGraph();
    }
    return (graph_tokens);
}

Loader::~Loader() {}
