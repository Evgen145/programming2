#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <random>
#include <future> // Додано заголовочний файл для роботи з багатопотоковістю

using namespace std;

struct Stats {
    map<string, int> word_frequencies;

    void operator += (const Stats& other) {
        for (const auto& pair : other.word_frequencies) {
            word_frequencies[pair.first] += pair.second;
        }
    }
};

set<string> ReadKeyWords(istream& is) {
    set<string> key_words;
    string word;
    while (is >> word) {
        key_words.insert(word);
    }
    return key_words;
}

vector<string> Split(const string& line) {
    istringstream iss(line);
    vector<string> words;
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(words));
    return words;
}

Stats ExploreLine(const set<string>& key_words, const string& line) {
    Stats result;
    vector<string> words = Split(line);
    for (const auto& word : words) {
        if (key_words.count(word)) {
            result.word_frequencies[word]++;
        }
    }
    return result;
}

Stats ExploreBatch(set<string> key_words, vector<string> lines) {
    Stats result;
    for (const auto& line : lines) {
        result += ExploreLine(key_words, line);
    }
    return result;
}

Stats ExploreKeyWordsSingleThread(const set<string>& key_words, istream& input) {
    Stats result;
    string line;
    while (getline(input, line)) {
        result += ExploreLine(key_words, line);
    }
    return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
    constexpr size_t BATCH_SIZE = 500;
    vector<future<Stats>> futures;

    while (input.peek() != EOF) {
        vector<string> batch;
        for (size_t i = 0; i < BATCH_SIZE && input.peek() != EOF; ++i) {
            string line;
            getline(input, line);
            batch.push_back(move(line));
        }
        futures.push_back(async(ExploreBatch, cref(key_words), move(batch)));
    }

    Stats result;
    for (auto& f : futures) {
        result += f.get(); // Отримуємо результат кожного асинхронного завдання і додаємо його до загального результату
    }

    return result;
}


void TestSplit() {
    const vector<string> expected1 = {
      "abc", "def", "ghi,", "!", "jklmnop-qrs,", "tuv"
    };
    ASSERT_EQUAL(Split("  abc def ghi, !  jklmnop-qrs, tuv"), expected1);

    const vector<string> expected2 = { "a", "b" };
    ASSERT_EQUAL(Split("a b      "), expected2);
    ASSERT_EQUAL(Split(""), vector<string>());
}

void TestBasic() {
    const set<string> key_words = { "softit", "rocks", "sucks", "all" };

    stringstream ss;
    ss << "this new softit service really rocks\n";
    ss << "It sucks when softit isn't available\n";
    ss << "10 reasons why softit is the best IT company\n";
    ss << "softit rocks others suck\n";
    ss << "Goondex really sucks, but softit rocks. Use softit\n";

    const auto stats = ExploreKeyWords(key_words, ss);
    const map<string, int> expected = {
      {"softit", 6},
      {"rocks", 2},
      {"sucks", 1}
    };
    ASSERT_EQUAL(stats.word_frequencies, expected);
}

void TestMtAgainstSt() {
    ifstream key_words_input("key_words.txt");
    const auto key_words_data = ReadKeyWords(key_words_input);
    const set<string> key_words(key_words_data.begin(), key_words_data.end());

    Stats st_stats, mt_stats;
    {
        ifstream text_input("text.txt");
        LOG_DURATION("Single thread");
        st_stats = ExploreKeyWordsSingleThread(key_words, text_input);
    }
    {
        ifstream text_input("text.txt");
        LOG_DURATION("Multi thread");
        mt_stats = ExploreKeyWords(key_words, text_input);
    }

    ASSERT_EQUAL(st_stats.word_frequencies, mt_stats.word_frequencies);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSplit);
    RUN_TEST(tr, TestBasic);
    RUN_TEST(tr, TestMtAgainstSt);
}
