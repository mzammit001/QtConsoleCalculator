#include "common.h"
#include "textresource.h"

#pragma once
class ExpressionLexer
{
private:
    std::string m_expr;
    std::vector<std::pair<std::string, std::string>> m_tokens;

public:
    ExpressionLexer(const char *expr);
    ~ExpressionLexer();

    void tokenize();
    const std::vector<std::pair<std::string, std::string>>& getTokens() const;
};

