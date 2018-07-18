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

    namespace Lexer {
        namespace Regex {
            static const char *Separator = "^(\\(|\\)|,|\\[|\\])";
            static const char *Identifier = "^([a-z_][a-z_0-9]*)";
            static const char *Keyword = "^(def|load|save|clear|vars|func)";
            static const char *Operator = "^(\\*\\*|\\*|\\+|\\-|\\/|\\%|\\=|\\<\\<|\\>\\>|\\||\\&|\\^|\\\\|\\~)";
            static const char *Literal = "^(0x[0-9a-f.]+|0o[0-7]+|0b[0-1]+|([1-9][0-9]*|0)(\\.[0-9]+)?)";
        }
    }
}
