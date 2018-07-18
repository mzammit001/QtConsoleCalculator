#include "expressionlexer.h"

ExpressionLexer::ExpressionLexer(const char *expr)
    : m_expr(expr)
{
    tokenize();
}

ExpressionLexer::~ExpressionLexer()
{
}

void ExpressionLexer::tokenize()
{
    using namespace TextResource::Lexer;
    
    // compiled regular expressions for tokenization
    std::vector<std::pair<std::string, std::regex>> regexPairs = {
        std::make_pair("KEYWORD", std::regex(Regex::Keyword, std::regex_constants::icase)),
        std::make_pair("SEPARATOR", std::regex(Regex::Separator)),
        std::make_pair("IDENTIFIER", std::regex(Regex::Identifier, std::regex_constants::icase)),
        std::make_pair("OPERATOR", std::regex(Regex::Operator)),
        std::make_pair("LITERAL", std::regex(Regex::Literal, std::regex_constants::icase))
    };

    std::string expr(m_expr);

    // tokenize until we reach the end of the expression
    while (expr.size()) {
        // skip if its a space character
        if (isspace(expr[0])) {
            expr = expr.substr(1);
            continue;
        }
        
        std::smatch sm;
        bool found = false;
        // try each regular expression
        for (const std::pair<std::string, std::regex> &pair : regexPairs) {
            if (std::regex_search(expr, sm, pair.second)) {
                // extract the token and label it
                m_tokens.push_back(std::make_pair(pair.first, sm[0]));
                // new expression is the suffix
                expr = sm.suffix();
                found = true;
                break;
            }
        }

        if (!found)
            throw std::domain_error("Lexer: Invalid token");
    }
}

const std::vector<std::pair<std::string, std::string>>& ExpressionLexer::getTokens() const
{
    return m_tokens;
}
