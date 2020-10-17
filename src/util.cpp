#include "util.h"
#include <iostream>
#include <iterator>

namespace Util {
    std::vector<char> load_file(char const* filename) {
        std::ifstream ifs(filename, std::ios_base::binary);
        ifs.unsetf(std::ios_base::skipws);
        return { std::istream_iterator<char>(ifs), std::istream_iterator<char>()};
    }
};
