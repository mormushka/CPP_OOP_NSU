#ifndef WORDCOUNTER_HPP_
#define WORDCOUNTER_HPP_

#include <map>
#include <vector>
#include <string>

#include "StringHelper.hpp"

class wc_error : public std::exception {
 private:
    std::string error_message;
 public:
    wc_error(const std::string &message): error_message(message) {}
    const char* what() const noexcept override;
};

class WordCounter {
 public:
    WordCounter() = default;
    void AddWord(const std::string& word);
    void Calculate();
    std::vector<StringHelper::WordInformation> GetCalculatedWords();

 private:
    std::map<std::string, size_t> counter_of_words;
    std::vector<StringHelper::WordInformation> calculated_words;
    size_t total_words = 0;
};

#endif //WORDCOUNTER_HPP_