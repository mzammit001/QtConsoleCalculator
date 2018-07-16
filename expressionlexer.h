#pragma once
#include "common.h"

class ExpressionLexer
{
private:
    std::string m_expr;

public:
    ExpressionLexer(const char *expr);
    ~ExpressionLexer();

    std::vector<std::string> getTokens();
};
