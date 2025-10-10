#ifndef STRINGHELPER_HPP_
#define STRINGHELPER_HPP_

#include <string>
#include <vector>
#include <algorithm>

namespace StringHelper
{
    static const size_t PRESICTION_VALUE = 6;
    static const size_t HUNDRED_PERCENTS = 100;

    struct WordInformation {
        std::string word;
        size_t count;
        double percents;
    };

    class StringHelper {
     public:
        static StringHelper& getInstance();
        void Parser(std::string &word);

        StringHelper(const StringHelper&) = delete;
        StringHelper& operator=(const StringHelper&) = delete;

     private:
        StringHelper() = default;
        ~StringHelper() = default;
    };

}

#endif // STRINGHELPER_HPP_