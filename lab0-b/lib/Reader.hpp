#ifndef READER_HPP_
#define READER_HPP_

#include <vector>
#include <string>
#include <fstream>

#include "StringHelper.hpp"

class ReaderCantOpenFileException : std::exception {
 private:
    std::string error_message;

 public:
    explicit ReaderCantOpenFileException(const std::string &message) : error_message(message) {}
    const char* what() const noexcept;
};

class ReaderCantCloseFileException : std::exception {
 private:
    std::string error_message;

 public:
    explicit ReaderCantCloseFileException(const std::string &message) : error_message(message) {}
    const char* what() const noexcept;
};

template <typename T>
class Reader {
 private:
    //std::string FileName;
    //std::vector <T> elements;
    std::ifstream in;
    std::string InputFileName_;
 public:
    Reader(std::string InputFileName = "input.txt");
    Reader(char *InputFileName = "input.txt");

    //std::vector<T> GetElements();
    template <typename Callback>
    void ProcessElements(Callback callback);

    //void ReadElements();

    ~Reader();
};

template <typename T>
Reader<T>::Reader(std::string inputFileName) {
    InputFileName_ = inputFileName;

    in.open(inputFileName);
    if (!in.is_open()) {
        throw ReaderCantOpenFileException("ERROR: Reader.cpp: Input file " + InputFileName_ + " can't be open\n");
    }
}


template <typename T>
Reader<T>::Reader(char *inputFileName) {
    InputFileName_ = inputFileName;

    in.open(inputFileName);
    if (!in.is_open()) {
        throw ReaderCantOpenFileException("ERROR: Reader.cpp: Input file " + InputFileName_ + " can't be open\n");
    }
}

template <typename T>
template <typename Callback>
void Reader<T>::ProcessElements(Callback callback) {
    T element;
    while(in >> element) {
        callback(element);
    }
}

template <>
template <typename Callback>
void Reader<std::string>::ProcessElements(Callback callback) {
    std::string str;
    StringHelper::StringHelper &helper = StringHelper::StringHelper::getInstance();
    while (in >> str) {
        helper.Parser(str);
        callback(str);
    }
}

/*
template <typename T>
std::vector<T> Reader<T>::GetElements() {
    return elements;
}

template <>
void Reader<std::string>::ReadElements() {
    std::string str;
    StringHelper::StringHelper &helper = StringHelper::StringHelper::getInstance();
    while (in >> str) {
        helper.Parser(str);
        elements.push_back(str);
    }
}

template <typename T>
void Reader<T>::ReadElements() {
    T element;
    while(in >> element) {
        elements.push_back(element);
    }
}
*/

template <typename T>
Reader<T>::~Reader() {
    in.close();
    if (in.is_open()) {
        std::cerr << "ERROR: Reader.cpp: Input file " + InputFileName_ + " can't be close\n";   
    }
}

const char* ReaderCantOpenFileException::what() const noexcept {
    return error_message.c_str();
}

const char* ReaderCantCloseFileException::what() const noexcept {
    return error_message.c_str();
}

#endif //READER_HPP_
