#include "expressionparser.h"

ExpressionParser::ExpressionParser(const std::vector<std::pair<std::string, std::string>>& tokens, ParseTable * pt)
    : m_tokens(tokens), m_pt(pt)
{
    finalizeTokens();
    parse();
}

ExpressionParser::~ExpressionParser()
{
}

void ExpressionParser::finalizeTokens()
{
    m_tokens.push_back(std::make_pair("END_SYMBOL", TermSymbol));
}

void ExpressionParser::parse()
{
    std::stack<Token> parseStack;

}

std::vector<std::string> ExpressionParser::getSyntaxTree()
{
    return std::vector<std::string>();
}
