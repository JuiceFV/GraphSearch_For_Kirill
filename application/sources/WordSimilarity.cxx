#include "WordSimilarity.h"

static std::vector<std::size_t> getHashedNgrams(const std::vector<std::string> &ngrams_vector)
{
    std::vector<std::size_t> hashed_ngrams_vector;
    for (auto &ngram : ngrams_vector) hashed_ngrams_vector.push_back(std::hash<std::string>{}(ngram));
    return hashed_ngrams_vector;
}

static std::vector<std::string> getNgrams(const std::string &word, const uint8_t &n)
{
    std::vector<std::string> ngrams_vector;
    for (uint8_t i = 0; (i + n) <= word.size(); i++) ngrams_vector.push_back(word.substr(i, n));
    return ngrams_vector;
}

static void printProbaTable(
    const std::unordered_map<std::string, std::vector<std::unordered_map<std::string, float>>> &proba_table,
    const std::vector<std::string> &possible_words)
{
    auto getWordsMaxLen = [&proba_table]() {
        {
            std::size_t len = 1;
            for (auto &field : proba_table)
                if (field.first.size() > len) len = field.first.size();
            return len;
        }
    };
    size_t max_word_len = getWordsMaxLen();
    std::cout << std::string(max_word_len, ' ') + " |";
    for (auto &ngram : proba_table.at(possible_words[0])[0])
        std::cout << ngram.first + std::string(std::to_string(ngram.second).size() - ngram.first.size(), ' ') + '|';
    std::cout << std::endl;
    for (auto &word : proba_table)
    {
        std::cout << word.first + std::string(max_word_len - word.first.size(), ' ') + " |";
        for (auto &ngram : word.second[0]) std::cout << std::to_string(ngram.second) + '|';
        std::cout << std::endl;
    }
}

static std::unordered_map<std::string, std::vector<std::unordered_map<std::string, float>>> calculateNgramsProba(
    const std::vector<std::string> &possible_words, const std::vector<size_t> &hashed_ngrams,
    const std::vector<std::string> &ngrams, const std::uint8_t n, const std::size_t num_embedded_ngrams,
    bool print_proba)
{
    std::unordered_map<std::string, std::vector<std::unordered_map<std::string, float>>> proba_table;
    for (auto &word : possible_words)
    {
        std::vector<std::size_t> word_hashed_ngrams = getHashedNgrams(getNgrams(word, n));
        proba_table[word].push_back({});
        for (unsigned int i = 0; i < hashed_ngrams.size(); i++)
            proba_table[word][0].insert(
                {{"'" + ngrams[i] + "'",
                  (float)(std::count(word_hashed_ngrams.begin(), word_hashed_ngrams.end(), hashed_ngrams[i])) /
                      word_hashed_ngrams.size()}});
    }

    if (print_proba)
    {
        std::cout << "Ngram's probabilities table" << std::endl;
        printProbaTable(proba_table, possible_words);
    }
    return proba_table;
}

static void logProba(std::unordered_map<std::string, std::vector<std::unordered_map<std::string, float>>> &proba_table,
                     bool print_proba, const std::vector<std::string> &possible_words)
{
    for (auto &word : proba_table)
    {
        for (auto &proba : word.second[0])
        {
            float proba_log = std::logf(proba.second);
            proba.second = std::isinf(proba_log) ? 0.0f : proba_log;
        }
    }
    if (print_proba)
    {
        std::cout << "Ngram's log of probabilities table" << std::endl;
        printProbaTable(proba_table, possible_words);
    }
}

static std::unordered_map<std::string, float> logSum(
    std::unordered_map<std::string, std::vector<std::unordered_map<std::string, float>>> &log_proba_table,
    bool print_proba)
{
    std::unordered_map<std::string, float> result;
    for (auto &word : log_proba_table)
    {
        float sum = 0.0f;
        for (auto &log_proba : word.second[0]) sum += log_proba.second;
        result[word.first] = sum;
    }
    if (print_proba)
    {
        auto printSumLogs = [&result]() {
            std::cout << "Log Sums:" << std::endl;
            for (auto &word : result) { std::cout << word.first << ": " << word.second << std::endl; }
        };
        printSumLogs();
    }
    return result;
}

static std::string selectWord(const std::unordered_map<std::string, float> &log_sums)
{
    // if two or more words have equal logs the first one will be chosen.
    std::string result = "";
    float min_log = 0.0f;
    for (auto &word : log_sums)
    {
        if (word.second < min_log)
        {
            min_log = word.second;
            result = word.first;
        }
    }
    if (min_log == 0.0f)
    {
        std::cerr << "The 'n' is too high, reduce the value of 'n'!";
        exit(1);
    }
    return result;
}

std::string smlrt::findeSimilarWord(std::vector<std::string> possible_words, const std::string &word, uint8_t n,
                                    bool print_proba)
{
    if (n < 1 || n >= word.size())
    {
        throw std::invalid_argument(
            "N-gram must be less than the size of a word and greater than 1! The default (n=2) set.");
        n = 2;
    }
    std::vector<std::string> ngrams = getNgrams(word, n);
    std::vector<std::size_t> hashed_ngrams = getHashedNgrams(ngrams);
    auto ngrams_proba =
        calculateNgramsProba(possible_words, hashed_ngrams, ngrams, n, word.size() - n + 1, print_proba);
    logProba(ngrams_proba, print_proba, possible_words);
    return selectWord(logSum(ngrams_proba, print_proba));
}
