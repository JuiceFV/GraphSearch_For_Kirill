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

namespace fs = std::filesystem;

class Loader
{
  public:
    Loader();                                                                        // constructor
    Loader(const std::string &path);                                                 // constructor 2
    std::unordered_map<std::string, std::vector<std::string>> getFileSource() const; // returns the graph's data as dict
    friend std::ostream &operator<<(std::ostream &out, const Loader &const obj)      // print graph's data
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
    std::unordered_map<std::string, std::vector<std::string>> tokens; // the dict which contains graph's data
    fs::path file_path;                                               // path to a file
    std::unordered_map<std::string, std::vector<std::string>> loadGraphFile(std::string path) const;
};
