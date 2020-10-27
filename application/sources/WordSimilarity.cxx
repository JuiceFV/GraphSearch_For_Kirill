#include "WordSimilarity.h"

// �������� ������������ n-grams, �.�. ���������� �� �������� �� �� ���������� �� ������
static std::vector<std::size_t> getHashedNgrams(const std::vector<std::string> &ngrams_vector)
{
    std::vector<std::size_t> hashed_ngrams_vector;
    // ������ ��������� ���������� id ������� n-gram � ����� ��� � ������
    for (auto &ngram : ngrams_vector) hashed_ngrams_vector.push_back(std::hash<std::string>{}(ngram));
    return hashed_ngrams_vector;
}

// ��������� ngram-� �� �����
static std::vector<std::string> getNgrams(const std::string &word, const uint8_t &n)
{
    std::vector<std::string> ngrams_vector;
    // ���� ��������� ������ � n � ����� � ��������������� ������
    // ������ ��� �� ���� �������, ���� ��������� �� ������ ��������� ������ �����
    for (uint8_t i = 0; (i + n) <= word.size(); i++) ngrams_vector.push_back(word.substr(i, n));
    return ngrams_vector;
}

// ������� ������ ������ ������������
static void printProbaTable(
    const std::unordered_map<std::string, std::vector<std::unordered_map<std::string, float>>> &proba_table,
    const std::vector<std::string> &possible_words)
{
    // �������� ����� ������������ �����
    // ����� ������� �� �����������
    auto getWordsMaxLen = [&proba_table]() {
        {
            std::size_t len = 1;
            for (auto &field : proba_table)
                if (field.first.size() > len) len = field.first.size();
            return len;
        }
    };
    size_t max_word_len = getWordsMaxLen();
    // ������� - ��� ngram-�, ������� ��
    std::cout << std::string(max_word_len, ' ') + " |";
    for (auto &ngram : proba_table.at(possible_words[0])[0])
        std::cout << ngram.first + std::string(std::to_string(ngram.second).size() - ngram.first.size(), ' ') + '|';
    std::cout << std::endl;
    // ������� ����� � ����������� (������� �������������) ������� ngram-a 
    for (auto &word : proba_table)
    {
        std::cout << word.first + std::string(max_word_len - word.first.size(), ' ') + " |";
        for (auto &ngram : word.second[0]) std::cout << std::to_string(ngram.second) + '|';
        std::cout << std::endl;
    }
}

// ������� ����������� ����������� (������� �������������) ��� ������� ngram'a
static std::unordered_map<std::string, std::vector<std::unordered_map<std::string, float>>> calculateNgramsProba(
    const std::vector<std::string> &possible_words, const std::vector<size_t> &hashed_ngrams,
    const std::vector<std::string> &ngrams, const std::uint8_t n, const std::size_t num_embedded_ngrams,
    bool print_proba)
{
    // ���������������� ������� ������������
    std::unordered_map<std::string, std::vector<std::unordered_map<std::string, float>>> proba_table;
    // ��� ������� ����� � "���������" ������ (��������) (�������� ���������� � ����� � �������)
    for (auto &word : possible_words)
    {
        // �� �������� ��� ����� ������������� �� ngram'�
        std::vector<std::size_t> word_hashed_ngrams = getHashedNgrams(getNgrams(word, n));
        proba_table[word].push_back({});

        // ��� ������� ngram'a � �����, � ������� �� ��������� ������
        for (unsigned int i = 0; i < hashed_ngrams.size(); i++)
            // �� ����������� �����������, ����������� ������� n/m,
            // ��� n - ���-�� ��� ��������� ngram'a � ��������� �����
            // � m - ����� ���-�� ngram'�� � ��������� �����
            proba_table[word][0].insert(
                {{"'" + ngrams[i] + "'",
                  (float)(std::count(word_hashed_ngrams.begin(), word_hashed_ngrams.end(), hashed_ngrams[i])) /
                      word_hashed_ngrams.size()}});
    }

    // ���� ��������� ���������� ������ ������������ - ������
    if (print_proba)
    {
        std::cout << "Ngram's probabilities table" << std::endl;
        printProbaTable(proba_table, possible_words);
    }
    return proba_table;
}

// ������������� �����������, �.�. � ������������� �������� �������� (��� ������� �������� �� ��������)
// ��� ��� ����� ��� � �� ������������ - ��� ������. � log(a*b) = log(a) + log(b)
static void logProba(std::unordered_map<std::string, std::vector<std::unordered_map<std::string, float>>> &proba_table,
                     bool print_proba, const std::vector<std::string> &possible_words)
{
    for (auto &word : proba_table)
    {
        for (auto &proba : word.second[0])
        {
            // �������������, � ��������� ������� logf ���������� -inf
            // �������� ��� �� 0.0
            float proba_log = std::logf(proba.second);
            proba.second = std::isinf(proba_log) ? 0.0f : proba_log;
        }
    }

    // ������ ������� ����������, ���� �����
    if (print_proba)
    {
        std::cout << "Ngram's log of probabilities table" << std::endl;
        printProbaTable(proba_table, possible_words);
    }
}

// �������� ���������, ��� � ��� ������� log(a*b) = log(a) + log(b)
// � ����� ������ a � b ��� �����������, � ��� �����, ����� � 
// ngram(a) ���������� � ngram(b) ����������, � ������������� 
// �� ����������� ����������� ������������ P(a*b) = P(a) * P(b)
static std::unordered_map<std::string, float> logSum(
    std::unordered_map<std::string, std::vector<std::unordered_map<std::string, float>>> &log_proba_table,
    bool print_proba)
{   
    // ���������
    std::unordered_map<std::string, float> result;
    for (auto &word : log_proba_table)
    {
        float sum = 0.0f;
        for (auto &log_proba : word.second[0]) sum += log_proba.second;
        result[word.first] = sum;
    }

    // ������ �������, ���� �����
    if (print_proba)
    {
        // � ���� ������ ����� ��������� ����� ����, ������� ����������� �����
        // � ������ ���� �������
        auto printSumLogs = [&result]() {
            std::cout << "Log Sums:" << std::endl;
            for (auto &word : result) { std::cout << word.first << ": " << word.second << std::endl; }
        };
        printSumLogs();
    }
    return result;
}

// �������� ����� ���������� �����
static std::string selectWord(const std::unordered_map<std::string, float> &log_sums)
{
    // ���� ��� ��� ����� ����� ����� ���������� ����� ����������, ����� ������� ������
    // ����� ����� ����������, ����� ������������ �������, ��� ������ �� ���� �����
    std::string result = "";
    // ����� ������������ ���������, �.�. log(a), ��� a: [0, 1]
    float min_log = 0.0f;
    for (auto &word : log_sums)
    {
        if (word.second < min_log)
        {
            min_log = word.second;
            result = word.first;
        }
    }

    // ���� ��� ��������� �����, ������ ngram ������� �������
    // ����� �� ���������� �� ����� � �� ��� ����...
    if (min_log == 0.0f)
    {
        std::cerr << "The 'n' is too high, reduce the value of 'n'!";
        exit(1);
    }
    return result;
}

// ������������ �������
// possible_words - ��������� ����� �� ������� ����� ���������� ����� ����������
// word - ����� � �������
// n - ����� ngram'a (�� ������� - 2)
// print_proba - ����� �� ������� ������� ������������ (�� ������� - ���, false)
std::string smlrt::findeSimilarWord(std::vector<std::string> possible_words, const std::string &word, uint8_t n,
                                    bool print_proba)
{
    // ��������� ��������� �� �������� ����� ngram'a
    if (n < 1 || n >= word.size())
    {
        throw std::invalid_argument(
            "N-gram must be less than the size of a word and greater than 1! The default (n=2) set.");
        n = 2;
    }
    // ����� ���� ����� �� ngram'�
    std::vector<std::string> ngrams = getNgrams(word, n);
    // �������� ���-�������� ������� ngram'a
    std::vector<std::size_t> hashed_ngrams = getHashedNgrams(ngrams);
    // �������� ������� ������������ ��� ������� ���������� ����� � ��� ������� ngram'a � ���� �����
    auto ngrams_proba =
        calculateNgramsProba(possible_words, hashed_ngrams, ngrams, n, word.size() - n + 1, print_proba);
    // ������� ��������� ������������ � ���������� �� � �������
    logProba(ngrams_proba, print_proba, possible_words);

    //���������� ��������� �����
    return selectWord(logSum(ngrams_proba, print_proba));
}
