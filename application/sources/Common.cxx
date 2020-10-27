#include "Common.hpp"

// ���������� ���������� ������ �� ���������/-��
std::vector<std::string> std::strSplit(std::string s, const std::vector<std::string> &delim)
{
    std::vector<std::string> result; // ���������������� ������ ����

    // ����� �������, ������� ����� ������ �� ���������
    auto sepWords = [&result, &s](const std::string delim) {
        size_t pos = 0;
        std::string token;
        // ���� ����� ��������
        while ((pos = s.find(delim)) != std::string::npos)
        {
            // ��������� ��������� � ������ � � ���������
            token = s.substr(0, pos);
            result.push_back(token);

            // ������� ��������� ��������� � �������� �� ������
            s.erase(0, pos + delim.length());
        }
        result.push_back(s);
    };

    // ��� ����� ����� ��� �������� ������ �������, ������� ����� ������
    // �� ������� ����������� ���������. � Graph.cxx ��� ����������
    for (auto &_delim : delim)
    {
        sepWords(_delim);
        // ���� � ������ ���������� ��������� ���������� ���������� 
        // �� ������ ������� �� �������. �.�. � ���� � ��������� 
        // ������ ������� ������������ ������ � ������ ����������
        // ����� strSplit("2->3") = {2,3}, �� � ���������� ������ 
        // ���� ������� ��� �����
        if (result.size() > 1) return result;
        // ������� ��������� �� ������
        result.clear();
    }

    // ���� ��������� �� ���������� ���������� ������ ����
    return {};
}
