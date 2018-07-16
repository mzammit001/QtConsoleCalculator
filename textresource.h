#pragma once
#include <vector>
#include <string>
#include <map>

namespace TextResource {
    namespace UIString {
        static const char *WindowTitle = "Qt Console Calculator";
    }

    namespace Grammar {
        static std::vector<std::string> basic = {
            "START -> T A", "A -> T A | epsilon", "T -> 0 | 1 | 2"
        };
    }
}

