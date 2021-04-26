#pragma once
#include "stb_image.h"
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

    class stbi_data {
    public:
        int width, height, nrChannels;
        unsigned char* data;
        stbi_data(const char* filename) {
                stbi_set_flip_vertically_on_load(true);
                data = stbi_load(filename, &width, &height, &nrChannels, 0);
                if (data == nullptr) {
                    throw std::runtime_error(
                        std::string("Failed to load texture ") + filename
                    );
                }
            }
        ~stbi_data() { stbi_image_free(data); };
    };
}
