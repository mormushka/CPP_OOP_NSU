#ifndef WRITER_HPP_
#define WRITER_HPP_

#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

#include "StringHelper.hpp"

class WriterCantOpenFileException : std::exception {
 private:
    std::string error_message;

 public:
    explicit WriterCantOpenFileException(const std::string &message) : error_message(message) {}
    const char* what() const noexcept;
};

class WriterCantCloseFileException : std::exception {
 private:
    std::string error_message;

 public:
    explicit WriterCantCloseFileException(const std::string &message) : error_message(message) {}
    const char* what() const noexcept;
};

template <typename T>
class Writer {
 private:
    std::ofstream out;

    std::string OutputFileName_;

 public:
    Writer(std::string outputFileName = "out.csv");
    Writer(char *outputFileName = "out.csv");

    void Write(std::vector<T> writed_vector);

    ~Writer();
};

template <typename T>
Writer<T>::Writer(std::string outputFileName) {
    OutputFileName_ = outputFileName;

    out.open(outputFileName);
    if (!out.is_open()) {
        throw WriterCantOpenFileException("ERROR: WordCounter.cpp: Output file " + OutputFileName_ + " can't be open\n");
    }
}

template <typename T>
Writer<T>::Writer(char *outputFileName) {
    OutputFileName_ = outputFileName;

    out.open(outputFileName);
    if (!out.is_open()) {
        throw WriterCantOpenFileException("ERROR: WordCounter.cpp: Output file " + OutputFileName_ + " can't be open\n");
    }
}

template<>
void Writer <StringHelper::WordInformation>::Write(std::vector<StringHelper::WordInformation> writed_vector) {
    StringHelper::StringHelper &helper = StringHelper::StringHelper::getInstance();
    for (const auto &x : writed_vector) {
        out << x.word;
        out << "," << x.count << ",";
        out <<  std::setprecision(StringHelper::PRESICTION_VALUE) << x.percents * StringHelper::HUNDRED_PERCENTS << "%" << std::endl;
    }
}

template <typename T>
void Writer<T>::Write(std::vector<T> writed_vector) {
    for (const auto &x : writed_vector) {
        out << x << std::endl;
    }
}

template <typename T>
Writer<T>::~Writer() {
    out.close();
    if (out.is_open()) {
        std::cerr << "ERROR: Writer.cpp: Output file " + OutputFileName_ + " can't be close\n";
    }
}

const char* WriterCantOpenFileException::what() const noexcept {
    return error_message.c_str();
}

const char* WriterCantCloseFileException::what() const noexcept {
    return error_message.c_str();
}

#endif //WRITER_HPP_
