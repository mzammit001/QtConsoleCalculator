#pragma once
#include "common.h"

namespace Util {
    namespace String {
        std::vector<std::string> split(const std::string &str, const char *delim) {
            std::vector<std::string> tokens;
            size_t pos = 0, old_pos = 0;

            while ((pos = str.find(delim, old_pos)) != std::string::npos) {
                tokens.push_back(str.substr(old_pos, pos - old_pos));
                old_pos = pos + strlen(delim);
            }

            if (old_pos != str.size()) {
                tokens.push_back(str.substr(old_pos, str.size() - old_pos));
            }

            return tokens;
        }

        std::string strip(std::string &str, const char delim) {
            size_t start = 0, end = str.size() - 1;

            while (str[start] == delim || str[end] == delim) {
                if (str[start] == delim)
                    start++;
                if (str[end] == delim)
                    end--;
            }

            return str.substr(start, end - start + 1);
        }
    }
}