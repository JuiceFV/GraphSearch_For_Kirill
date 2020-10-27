#include "WordSimilarity.h"

// получаем хэшированные n-grams, т.к. сравнивать их напрямую на не английском не вайдет
static std::vector<std::size_t> getHashedNgrams(const std::vector<std::string> &ngrams_vector)
{
    std::vector<std::size_t> hashed_ngrams_vector;
    // просто извлекаем уникальный id каждого n-gram и пушим его в вектор
    for (auto &ngram : ngrams_vector) hashed_ngrams_vector.push_back(std::hash<std::string>{}(ngram));
    return hashed_ngrams_vector;
}

// извлекаем ngram-ы из слова
static std::vector<std::string> getNgrams(const std::string &word, const uint8_t &n)
{
    std::vector<std::string> ngrams_vector;
    // берём подстроку длиной в n и пушим в резултатирующий вектор
    // делаем это до того момента, пока подстрока не станет последней частью слова
    for (uint8_t i = 0; (i + n) <= word.size(); i++) ngrams_vector.push_back(word.substr(i, n));
    return ngrams_vector;
}

// функция вывода тыблиц вероятностей
static void printProbaTable(
    const std::unordered_map<std::string, std::vector<std::unordered_map<std::string, float>>> &proba_table,
    const std::vector<std::string> &possible_words)
{
    // получаем слово максимальной длины
    // чтобы таблица не разъехалась
    auto getWordsMaxLen = [&proba_table]() {
        {
            std::size_t len = 1;
            for (auto &field : proba_table)
                if (field.first.size() > len) len = field.first.size();
            return len;
        }
    };
    size_t max_word_len = getWordsMaxLen();
    // колонки - это ngram-ы, выводим их
    std::cout << std::string(max_word_len, ' ') + " |";
    for (auto &ngram : proba_table.at(possible_words[0])[0])
        std::cout << ngram.first + std::string(std::to_string(ngram.second).size() - ngram.first.size(), ' ') + '|';
    std::cout << std::endl;
    // выводим слова и вероятности (частоту встречаемости) каждого ngram-a 
    for (auto &word : proba_table)
    {
        std::cout << word.first + std::string(max_word_len - word.first.size(), ' ') + " |";
        for (auto &ngram : word.second[0]) std::cout << std::to_string(ngram.second) + '|';
        std::cout << std::endl;
    }
}

// функция высчитывает вероятности (частоту встречаемости) для каждого ngram'a
static std::unordered_map<std::string, std::vector<std::unordered_map<std::string, float>>> calculateNgramsProba(
    const std::vector<std::string> &possible_words, const std::vector<size_t> &hashed_ngrams,
    const std::vector<std::string> &ngrams, const std::uint8_t n, const std::size_t num_embedded_ngrams,
    bool print_proba)
{
    // результатирующая таблица вероятностей
    std::unordered_map<std::string, std::vector<std::unordered_map<std::string, float>>> proba_table;
    // для каждого слова в "возможных" словах (шаблонах) (смотрите объяснение в файле с графами)
    for (auto &word : possible_words)
    {
        // мы получаем эти слова раздробленные на ngram'ы
        std::vector<std::size_t> word_hashed_ngrams = getHashedNgrams(getNgrams(word, n));
        proba_table[word].push_back({});

        // для каждого ngram'a в слове, в котором мы допустили ошибку
        for (unsigned int i = 0; i < hashed_ngrams.size(); i++)
            // мы высчитываем вероятность, посредством деления n/m,
            // где n - кол-во раз появления ngram'a в шаблонном слове
            // а m - общее кол-во ngram'ов в шаблонном слове
            proba_table[word][0].insert(
                {{"'" + ngrams[i] + "'",
                  (float)(std::count(word_hashed_ngrams.begin(), word_hashed_ngrams.end(), hashed_ngrams[i])) /
                      word_hashed_ngrams.size()}});
    }

    // если требуется нарисовать табицу вероятностей - рисуем
    if (print_proba)
    {
        std::cout << "Ngram's probabilities table" << std::endl;
        printProbaTable(proba_table, possible_words);
    }
    return proba_table;
}

// логарифмируем вероятности, т.к. с вероятностями неудобно работать (они слишком маленьки по значению)
// так нам нужно ещё и их произведение - ещё меньше. А log(a*b) = log(a) + log(b)
static void logProba(std::unordered_map<std::string, std::vector<std::unordered_map<std::string, float>>> &proba_table,
                     bool print_proba, const std::vector<std::string> &possible_words)
{
    for (auto &word : proba_table)
    {
        for (auto &proba : word.second[0])
        {
            // логарифмируем, в некоторых случаях logf возвращает -inf
            // заменяем его на 0.0
            float proba_log = std::logf(proba.second);
            proba.second = std::isinf(proba_log) ? 0.0f : proba_log;
        }
    }

    // рисуем таблицу логарифмов, если нужно
    if (print_proba)
    {
        std::cout << "Ngram's log of probabilities table" << std::endl;
        printProbaTable(proba_table, possible_words);
    }
}

// сумируем логарифмы, как я уже говорил log(a*b) = log(a) + log(b)
// в нашем случае a и b это вероятности, а нам нужно, чтобы и 
// ngram(a) встретился и ngram(b) встретился, а следовательно 
// по определению независимых вероятностей P(a*b) = P(a) * P(b)
static std::unordered_map<std::string, float> logSum(
    std::unordered_map<std::string, std::vector<std::unordered_map<std::string, float>>> &log_proba_table,
    bool print_proba)
{   
    // суммируем
    std::unordered_map<std::string, float> result;
    for (auto &word : log_proba_table)
    {
        float sum = 0.0f;
        for (auto &log_proba : word.second[0]) sum += log_proba.second;
        result[word.first] = sum;
    }

    // рисуем таблицу, если нужно
    if (print_proba)
    {
        // у меня раньще здесь доффигища всего было, поэтому импользовал лябду
        // а сейчас лень удалять
        auto printSumLogs = [&result]() {
            std::cout << "Log Sums:" << std::endl;
            for (auto &word : result) { std::cout << word.first << ": " << word.second << std::endl; }
        };
        printSumLogs();
    }
    return result;
}

// выбираем более подходящее слово
static std::string selectWord(const std::unordered_map<std::string, float> &log_sums)
{
    // если два или более слова имеют одинаковую сумму логарифмов, будет выбрано первое
    // потом можно доработать, чтобы пользователь выбирал, что именно он имел ввиду
    std::string result = "";
    // поиск минимального логарифма, т.к. log(a), где a: [0, 1]
    float min_log = 0.0f;
    for (auto &word : log_sums)
    {
        if (word.second < min_log)
        {
            min_log = word.second;
            result = word.first;
        }
    }

    // если все логарифмы равны, значит ngram слишком большой
    // может он охватывает всё слово а не его чать...
    if (min_log == 0.0f)
    {
        std::cerr << "The 'n' is too high, reduce the value of 'n'!";
        exit(1);
    }
    return result;
}

// объёдиняющая функция
// possible_words - шаблонные слова из которых будет выбираться более подходящее
// word - слово с ошибкой
// n - длина ngram'a (по дефолту - 2)
// print_proba - нужно ли чертить таблицу вероятностей (по дефолту - нет, false)
std::string smlrt::findeSimilarWord(std::vector<std::string> possible_words, const std::string &word, uint8_t n,
                                    bool print_proba)
{
    // проверяем правильно ли передана длина ngram'a
    if (n < 1 || n >= word.size())
    {
        throw std::invalid_argument(
            "N-gram must be less than the size of a word and greater than 1! The default (n=2) set.");
        n = 2;
    }
    // делим наше слово на ngram'ы
    std::vector<std::string> ngrams = getNgrams(word, n);
    // получаем хэш-значения каждого ngram'a
    std::vector<std::size_t> hashed_ngrams = getHashedNgrams(ngrams);
    // получаем таблицу вероятностей для каждого шаблонного слова и для каждого ngram'a в этом слове
    auto ngrams_proba =
        calculateNgramsProba(possible_words, hashed_ngrams, ngrams, n, word.size() - n + 1, print_proba);
    // считаем логарифмы вероятностей и записываем их в таблицу
    logProba(ngrams_proba, print_proba, possible_words);

    //возвращаем выбранное слово
    return selectWord(logSum(ngrams_proba, print_proba));
}
