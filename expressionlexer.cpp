#include "expressionlexer.h"

ExpressionLexer::ExpressionLexer(const char *expr)
    : m_expr(expr)
{

}

std::vector<std::string> ExpressionLexer::getTokens()
{
    return std::vector<std::string>();
}
