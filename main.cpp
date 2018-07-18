#include "common.h"
#include <QApplication>
#include "textresource.h"
#include "grammar.h"

int main(int argc, char **argv)
{
    //ExpressionLexer lex("a = 5 ** (c + 2) % x");
    //ExpressionLexer lex("sqrt(5 ** 2) + f(10, 50)");
    ExpressionLexer lex("0xabcd0o00");
    auto tokens = lex.getTokens();

    for (const auto &t : tokens) {
        std::cout << "[ " << t.first << ", " << t.second << " ]" << std::endl;
    }
    //QApplication qapp(argc, argv);
    //Grammar g = Grammar(std::string(StartSymbol), TextResource::Grammar::basic);
    //const ParseTable &pt = g.getParseTable();

    //auto terms = pt.getTerminals(StartSymbol);
    //
    //for (const auto &s : terms) {
    //    std::cout << s << std::endl;
    //}

    //return qapp.exec();
    return 0;
}
