#include "grammar.h"

using namespace Util;

Grammar::Grammar(std::string start, StringVector grammar)
    : m_start(start), m_parseTable(nullptr)
{
    makeSimpleGrammar(grammar);
    generateParseTable();
}

Grammar::~Grammar()
{
    delete m_parseTable;
}

const ParseTable & Grammar::getParseTable() const
{
    return *m_parseTable;
}

void Grammar::generateParseTable()
{
    StringSetMap firstSet = makeFirstSet(m_start);
    StringSetMap followSet = makeFollowSet(m_start, firstSet);
    std::vector<TransformationTableEntry> tt = makeTransformationTable(m_start, firstSet, followSet);

    m_parseTable = new ParseTable(m_start, tt);
}

std::vector<TransformationTableEntry> Grammar::makeTransformationTable(std::string start, StringSetMap & firstSet, StringSetMap & followSet)
{
    std::vector<TransformationTableEntry> vtte;
    
    for (const auto &entry : m_grammar) {
        std::vector<std::string> symbols = String::split(entry.second, " ");

        std::vector<std::string> first;
        std::copy(firstSet[symbols[0]].begin(), firstSet[symbols[0]].end(), std::back_inserter(first));
        
        std::vector<std::string> follow;
        
        if (firstSet[symbols[0]].count(NullSymbol) || entry.first == start) {
            std::copy(followSet[entry.first].begin(), followSet[entry.first].end(), std::back_inserter(follow));
        }

        vtte.push_back(TransformationTableEntry(entry, first, follow));
    }
    
    return vtte;
}

StringSetMap Grammar::makeFirstSet(std::string start)
{
    StringSetMap firstSet;
    makeFirstSetRecur(m_start, firstSet);

    return firstSet;
}

StringSetMap Grammar::makeFollowSet(std::string start, StringSetMap & firstSet)
{
    StringSetMap followSet;
    // rule 1, start_symbol = { $ }
    followSet[start].insert(TermSymbol);
    makeFollowSetRecur(start, firstSet, followSet);

    // calculate the follow set for variables \ start
    for (const auto &v : m_variables) {
        if (v != start)
            makeFollowSetRecur(v, firstSet, followSet);
    }

    return followSet;
}

void Grammar::makeFirstSetRecur(std::string var, StringSetMap & firstSet)
{
    // terminal case
    if (std::find(m_variables.begin(), m_variables.end(), var) == m_variables.end()) {
        firstSet[var].insert(var);
        return;
    }

    // get an iterator for all the rules starting with var
    auto range = m_grammar.equal_range(var);

    for (auto it = range.first; it != range.second; ++it) {
        StringPair sp = *it;
        if (!firstSet.count(var))
            firstSet[var] = std::set<std::string>();

        StringVector prods = productionToSymbols(sp.second);
        for (const auto &p : prods) {
            if (!firstSet.count(p))
                makeFirstSetRecur(p, firstSet);
        }

        bool epsilon_prev = false;

        // X -> epsilon
        if (prods.size() == 1 && prods[0] == NullSymbol) {
            firstSet[var].insert(NullSymbol);
        }
        else {
            for (const auto &p : prods) {
                // FIRST(X) = FIRST(X) U FIRST(Y1) U ... U FIRST(Yk)
                std::copy(firstSet[p].begin(), firstSet[p].end(), std::inserter(firstSet[var], firstSet[var].begin()));
                // epsilon !E FIRST(Yk)
                if (std::find(firstSet[p].begin(), firstSet[p].end(), NullSymbol) == firstSet[p].end()) {
                    epsilon_prev = false;
                    break;
                }

                epsilon_prev = true;
            }
        }

        if (epsilon_prev)
            firstSet[var].insert(NullSymbol);
    }
}

void Grammar::makeFollowSetRecur(std::string var, StringSetMap & firstSet, StringSetMap & followSet)
{
    std::vector<StringPair> rules = findRulesWithSymbol(var);

    if (!followSet.count(var))
        followSet[var] = {};

    for (const auto &r : rules) {
        StringVector syms = productionToSymbols(r.second);

        size_t idx = 0;
        for (size_t i = 0; i < syms.size(); ++i) {
            if (syms[i] == var)
                idx = i;
        }

        // find FOLLOW(A) in A -> aBb if doesnt exist
        if (!followSet.count(r.first))
            makeFollowSetRecur(r.first, firstSet, followSet);
        
        // case: A -> aB
        if (idx == syms.size() - 1) {
            // FOLLOW(B) = FOLLOW(A) U FOLLOW(B)
            //std::set<std::string> tmp(followSet[r.first]);
            std::copy(followSet[r.first].begin(), followSet[r.first].end(), std::inserter(followSet[var], followSet[var].begin()));
            //followSet[var].merge(tmp);
        }
        // case: A -> aBb
        else {
            std::set<std::string> firstMinusNull;

            for (const auto &s : firstSet[syms[idx + 1]])
                if (s != NullSymbol)
                    firstMinusNull.insert(s);
            
            // FOLLOW(B) = FOLLOW(B) U FIRST(b) \ { epsilon }
            std::copy(firstMinusNull.begin(), firstMinusNull.end(), std::inserter(followSet[var], followSet[var].begin()));
            //followSet[var].merge(firstMinusNull);
            
            // FOLLOW(B) = FOLLOW(A) U FOLLOW(B)
            if (firstSet[syms[idx + 1]].count(NullSymbol)) {
                std::copy(followSet[r.first].begin(), followSet[r.first].end(), std::inserter(followSet[var], followSet[var].begin()));
                //followSet[var].merge(followSet[r.first]);
            }
        }

    }
}

void Grammar::makeVariables()
{
    std::set<std::string> vars;

    // find unique variables
    for (const auto &pair : m_grammar) {
        vars.insert(pair.first);
    }

    std::copy(vars.begin(), vars.end(), std::back_inserter(m_variables));
}

void Grammar::makeTerminals()
{
    std::set<std::string> terms;

    // find unique variables
    for (const auto &pair : m_grammar) {
        std::vector<std::string> symbols = productionToSymbols(pair.second);
        // go through all the symbols
        for (const auto &sym : symbols) {
            // if not a variable or null symbol (epsilon)
            if (std::find(m_variables.begin(), m_variables.end(), sym) == m_variables.end() || sym == NullSymbol) {
                terms.insert(sym);
            }
        }
    }

    std::copy(terms.begin(), terms.end(), std::back_inserter(m_terminals));
}

void Grammar::makeSimpleGrammar(const StringVector &grammar)
{
    for (auto &s : grammar) {
        // split the rule into variable, production
        std::vector<std::string> rule = String::split(s, " -> ");
        // split the production into components
        std::vector<std::string> prods = String::split(rule[1], " | ");

        for (auto &p : prods)
            m_grammar.insert(std::make_pair(rule[0], p));
    }

    // make the variables and terminals
    makeVariables();
    makeTerminals();
}

StringVector Grammar::productionToSymbols(std::string prod)
{
    return String::split(prod, " ");
}

std::vector<StringPair> Grammar::findRulesWithSymbol(std::string symbol)
{
    std::vector<StringPair> rules;

    for (const auto &p : m_grammar) {
        StringVector symbols = productionToSymbols(p.second);

        if (std::find(symbols.begin(), symbols.end(), symbol) != symbols.end()) {
            rules.push_back(p);
        }
    }

    return rules;
}
