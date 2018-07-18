#pragma once
#include "common.h"
#include "parsetable.h"
#include "token.h"

class ExpressionParser
{
private:
    std::vector<std::pair<std::string, std::string>> m_tokens;
    ParseTable *m_pt;

private:
    void finalizeTokens();

public:
    ExpressionParser(const std::vector<std::pair<std::string, std::string>> &tokens, ParseTable *pt);
    ~ExpressionParser();

    void parse();

    std::vector<std::string> getSyntaxTree();
};
