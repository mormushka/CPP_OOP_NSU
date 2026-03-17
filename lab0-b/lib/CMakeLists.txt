add_library(wc_lib
    ./WordCounter.cpp
    ./StringHelper.cpp
    ./WordCounter.hpp
    ./Reader.hpp
    ./StringHelper.hpp
    ./Writer.hpp
)

set_target_properties(wc_lib PROPERTIES LINKER_LANGUAGE CXX)