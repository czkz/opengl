#pragma once
#include <filesystem>
#include <fstream>
#include <set>
#include <stack>
#include <string_view>

namespace {
    /// Parses include directives, use each instance once!
    class _ShaderParser {
        public:
            static constexpr std::string_view directive = "##include ";
        private:
            std::set<std::filesystem::path> visited;
            std::stack<std::filesystem::path> include_stack;
        public:
            std::string parse(std::filesystem::path file) {
                std::string ret;
                parse_into(ret, file);
                return ret;
            }
            void parse_into(std::string& out, std::filesystem::path filename) {

                std::ifstream f(filename);
                if (!f.good()) {
                    std::ostringstream ss;
                    ss << "Couldn't find file \"";
                    ss << std::string(std::move(filename)) << "\"";
                    if (!include_stack.empty()) { ss << "\n"; }
                    while (!include_stack.empty()) {
                        ss << "included from " << include_stack.top() << "\n";
                        include_stack.pop();
                    }
                    throw std::runtime_error(ss.str());
                }
                include_stack.push(filename);

                std::string line;
                while (std::getline(f, line)) {
                    if (line.starts_with(directive)) {
                        const auto include_arg = std::string_view(line)
                            .substr(directive.size());
                        std::filesystem::path include_file =
                            filename.replace_filename(include_arg);
                        if (!visited.contains(include_file)) {
                            visited.insert(include_file);
                            parse_into(out, include_file);
                        }
                    } else {
                        out += line;
                        out += '\n';
                    }
                }
            }
    };
}

namespace gl::_ {
    /// preprocess_shader handles includes in shaders.
    std::string preprocess_shader(std::filesystem::path file) {
        return _ShaderParser().parse(std::move(file));
    }
}
