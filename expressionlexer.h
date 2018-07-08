#pragma once
#include "common.h"

class ExpressionLexer
{

public:
    ExpressionLexer(const char *expr);
    ~ExpressionLexer();

    std::vector<std::string> getTokens();
};
