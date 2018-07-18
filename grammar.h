#pragma once
#include "common.h"
#include "parsetable.h"
#include "expressionlexer.h"

class Grammar
{
private:
    std::multimap<std::string, std::string> m_grammar;
    StringVector m_variables;
    StringVector m_terminals;
    ParseTable *m_parseTable;
    std::string m_start;
    
private:
    void generateParseTable();
    std::vector<TransformationTableEntry> makeTransformationTable(std::string start, StringSetMap &firstSet, StringSetMap &followSet);

    StringSetMap makeFirstSet(std::string start);
    StringSetMap makeFollowSet(std::string start, StringSetMap &firstSet);
    void makeFirstSetRecur(std::string var, StringSetMap &firstSet);
    void makeFollowSetRecur(std::string var, StringSetMap &firstSet, StringSetMap &followSet);

    void makeVariables();
    void makeTerminals();
    void makeSimpleGrammar(const StringVector &grammar);
    StringVector productionToSymbols(std::string prod);
    std::vector<StringPair> findRulesWithSymbol(std::string symbol);

public:
    Grammar(std::string start, StringVector grammar);
    ~Grammar();

    const ParseTable &getParseTable() const;
};

