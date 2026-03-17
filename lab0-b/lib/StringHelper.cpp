#include "StringHelper.hpp"

namespace StringHelper {

StringHelper& StringHelper::getInstance() {
    static StringHelper instance;
    return instance;
}

void StringHelper::Parser(std::string &word) {
    for (size_t i = 0; i < word.size(); i++) {
        if (isalnum(static_cast<unsigned char>(word.at(i)))) {
            word.at(i) = std::tolower(word.at(i));
            continue;
        }
        word.erase(i, 1);
    }
}

}