#pragma once
#include <string>
#include <fstream>

namespace file_utils {
    std::string readFile(const char* path) {
        std::ifstream f(path);
        std::string ret;
        if (!f.good()) {
            return ret;
        }
        f.seekg(0, std::ios::end);
        auto size = f.tellg();
        f.seekg(0, std::ios::beg);
        ret.resize(size, '\0');
        f.read(ret.data(), size);
        return ret;
    }
}
