#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <deque>
#include <stack>
#include <algorithm>
#include <regex>
#include <string>
#include <sstream>
#include <string_view>
#include <map>
#include <set>
#include <unordered_set>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <cstring>
#include <QString>
#include "util.h"

static constexpr const char *TermSymbol = "$";
static constexpr const char *NullSymbol = "epsilon";
static constexpr const char *StartSymbol = "START";

using StringVectorMap = std::map<std::string, std::vector<std::string>>;
using StringSetMap = std::map<std::string, std::set<std::string>>;
using StringVector = std::vector<std::string>;
using StringPair = std::pair<std::string, std::string>;