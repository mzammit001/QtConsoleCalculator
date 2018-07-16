#include "common.h"
#include <QApplication>
#include "textresource.h"
#include "grammar.h"

int main(int argc, char **argv)
{
    //QApplication qapp(argc, argv);
    Grammar g = Grammar(std::string(StartSymbol), TextResource::Grammar::basic);
    const ParseTable &pt = g.getParseTable();

    auto terms = pt.getTerminals(StartSymbol);
    
    for (const auto &s : terms) {
        std::cout << s << std::endl;
    }

    //return qapp.exec();
    return 0;
}
