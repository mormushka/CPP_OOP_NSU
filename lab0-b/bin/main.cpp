#include <iostream>
#include <map>

#include "StringHelper.hpp"
#include "WordCounter.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

static const size_t REQUIRED_NUMBER_OF_ARGUMENTS = 3;

class MainCountArgumentsException : public std::exception {
 private:
    std::string error_message;

 public:
    explicit MainCountArgumentsException(const std::string &message) : error_message(message) {}
    const char* what() const noexcept;
};

int main(int argc, char* argv[]) {
    if (argc != REQUIRED_NUMBER_OF_ARGUMENTS) {
        throw MainCountArgumentsException("ERROR: main.cpp: check on count arguments");
    }
    
    StringHelper::StringHelper &helper = StringHelper::StringHelper::getInstance();

    Reader<std::string> reader(argv[1]);
    WordCounter counter;
    
    reader.ProcessElements([&counter](const std::string& word) {
        counter.AddWord(word);
    });

    counter.Calculate();
    std::vector <StringHelper::WordInformation> calculated_words = counter.GetCalculatedWords();

    Writer <StringHelper::WordInformation> writer(argv[2]);
    writer.Write(calculated_words);

    return 0;
}

const char* MainCountArgumentsException::what() const noexcept {
    return error_message.c_str();
}