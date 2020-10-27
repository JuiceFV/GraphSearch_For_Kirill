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

// ��. ��� ��� �������� �������
namespace fs = std::filesystem;

class Loader
{
  public:
    // ��������� ����������� ���������� ��������� ������,
    // ���� �� �������� ���������, ���� ���� ��� ������� �������
    Loader();

    // ������� �����������, ��������� ���� �� ����� � ���� ���������.
    Loader(const std::string &path);

    // ���������� ��� ������ ��� ����� � ���� ���-�������
    // ��� ���� - ��� �����, � �� ����� - ��� ������ ��� ������
    // �������� hm["vertices"]={"0", "1", ..., "n"} ��� hm["type"] = "undirected"
    std::unordered_map<std::string, std::vector<std::string>> getFileSource() const;
    friend std::ostream &operator<<(std::ostream &out,
                                    const Loader &const obj) // ������� ������ ����� � ����������������� ����
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
    std::unordered_map<std::string, std::vector<std::string>> tokens; // ���������������� ������� ������ ��� �����
    fs::path file_path;                                               // ���� � �����
    // ������� �������, ������� ������������ ����
    std::unordered_map<std::string, std::vector<std::string>> loadGraphFile(std::string path) const;
};
