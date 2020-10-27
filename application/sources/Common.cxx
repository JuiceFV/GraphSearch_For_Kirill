#include "Common.hpp"

// реализация разделения строки по дилиметру/-ам
std::vector<std::string> std::strSplit(std::string s, const std::vector<std::string> &delim)
{
    std::vector<std::string> result; // результатирующий вектор слов

    // лябда функция, которая делит строку по делиметру
    auto sepWords = [&result, &s](const std::string delim) {
        size_t pos = 0;
        std::string token;
        // если нашли делиметр
        while ((pos = s.find(delim)) != std::string::npos)
        {
            // извлекаем подстроку и кидаем её в результат
            token = s.substr(0, pos);
            result.push_back(token);

            // убераем найденную подстроку и делиметр из строки
            s.erase(0, pos + delim.length());
        }
        result.push_back(s);
    };

    // для своих целей мне пришлось делать функцию, которая делит строку
    // по первому подошедшему делиметру. В Graph.cxx это пнадобится
    for (auto &_delim : delim)
    {
        sepWords(_delim);
        // если в строке появляется несколько подходящих делиметров 
        // то строка делится по первому. Т.к. у меня в программе 
        // данная функция используется только в случае разделения
        // нодов strSplit("2->3") = {2,3}, то в результате должно 
        // быть минимум два слова
        if (result.size() > 1) return result;
        // очищаем результат от мусора
        result.clear();
    }

    // если разделить не получилось возвращаем пустой лист
    return {};
}
