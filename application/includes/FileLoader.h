#pragma once

#include <exception>
#include <filesystem>
#include <unordered_map>
#include <vector>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Globals.h"

// пр. имён для файловой системы
namespace fs = std::filesystem;

class Loader
{
  public:
    // дефолтный конструктор подгружает дефолтный конфиг,
    // дабы не обрывать программу, если файл был передан неверно
    Loader();

    // обычный конструктор, принимает путь до файла в виде пармаетра.
    Loader(const std::string &path);

    // возвращает все данные для графа в виде хэш-таблицы
    // Где ключ - это токен, а от ключа - это данные для граффа
    // например hm["vertices"]={"0", "1", ..., "n"} или hm["type"] = "undirected"
    std::unordered_map<std::string, std::vector<std::string>> getFileSource() const;
    friend std::ostream &operator<<(std::ostream &out,
                                    const Loader &const obj) // выводит данные графа в отформатированном виде
    {
        out << "\nType:" << std::endl << "\t" << obj.tokens.at("type")[0] << std::endl << "Vertices:" << std::endl;
        for (auto &vertex : obj.tokens.at("vertices")) out << "\t" << vertex << std::endl;
        out << "Edges:" << std::endl;
        for (auto &edge : obj.tokens.at("edges")) out << "\t" << edge << std::endl;
        out << "Start:" << std::endl
            << "\t" << obj.tokens.at("start")[0] << std::endl
            << "Goal:" << std::endl
            << "\t" << obj.tokens.at("goal")[0] << std::endl;
        return out;
    }
    ~Loader();

  private:
    std::unordered_map<std::string, std::vector<std::string>> tokens; // результатирующий словарь данных для графа
    fs::path file_path;                                               // путь к файлу
    // неявная функция, которая обрабатывает файл
    std::unordered_map<std::string, std::vector<std::string>> loadGraphFile(std::string path) const;
};
