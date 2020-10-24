#pragma once

#include <exception>
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <cmath>

namespace smlrt
{
    std::string findeSimilarWord(std::vector<std::string> possible_words, const std::string &word, uint8_t n=2,
                                        bool print_proba=false);
}
