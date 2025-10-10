#include "WordCounter.hpp"

const char* wc_error::what() const noexcept {
    return error_message.c_str();
}

WordCounter::WordCounter(std::vector<std::string> words) {
    _words = words;
}

void WordCounter::Calculate() {
    StringHelper::StringHelper &helper = StringHelper::StringHelper::getInstance();

    for (const auto &word : _words) {
        counter_of_words[word]++;
    }

    for (const auto &x : counter_of_words) {
        std::string word = x.first;
        word.front() = toupper(static_cast<unsigned char>(word.front()));
        calculated_words.push_back({word, x.second, x.second / (double)_words.size()});
    }

    sort(calculated_words.begin(), calculated_words.end(),
    [](const StringHelper::WordInformation &a, const StringHelper::WordInformation &b) {
        return a.count > b.count;
    });
}

std::vector<StringHelper::WordInformation> WordCounter::GetCalculatedWords() {
    return calculated_words;
}