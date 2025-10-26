#include "WordCounter.hpp"

const char* wc_error::what() const noexcept {
    return error_message.c_str();
}

void WordCounter::AddWord(const std::string& word) {
    counter_of_words[word]++;
    total_words++;
}

void WordCounter::Calculate() {
    for (const auto &pair : counter_of_words) {
        std::string word = pair.first;
        if (!word.empty()) {
            word.front() = std::toupper(static_cast<unsigned char>(word.front()));
        }
        calculated_words.push_back({
            word, 
            pair.second, 
            static_cast<double>(pair.second) / total_words
        });
    }

    std::sort(calculated_words.begin(), calculated_words.end(),
        [](const StringHelper::WordInformation &a, const StringHelper::WordInformation &b) {
            return a.count > b.count;
        });
}

std::vector<StringHelper::WordInformation> WordCounter::GetCalculatedWords() {
    return calculated_words;
}