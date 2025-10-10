#ifndef WORDCOUNTER_HPP_
#define WORDCOUNTER_HPP_

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <iomanip>

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
    explicit WordCounter(std::vector<std::string> words);
    void Calculate();
    std::vector<StringHelper::WordInformation> GetCalculatedWords();
    ~WordCounter() = default;

 private:
    std::vector<std::string> _words;
    std::map<std::string, size_t> counter_of_words;
    std::vector<StringHelper::WordInformation> calculated_words;
};

#endif //WORDCOUNTER_HPP_