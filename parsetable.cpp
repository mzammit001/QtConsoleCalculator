#include "parsetable.h"
#include "util.h"

TransformationTableEntry::TransformationTableEntry(std::string& prod, std::vector<std::string>& first, std::vector<std::string>& follow)
    : m_prod(prod), m_first(first), m_follow(follow)
{
}

const std::string & TransformationTableEntry::getProduction() const
{
    return m_prod;
}

const std::vector<std::string>& TransformationTableEntry::getFirstSet() const
{
    return m_first;
}

const std::vector<std::string>& TransformationTableEntry::getFollowSet() const
{
    return m_follow;
}

ParseTable::KeyProxy::KeyProxy(const ParseTable & pt, int prodIndex)
    : m_pt(pt), m_prodIndex(prodIndex)
{
}

std::string ParseTable::KeyProxy::operator[](const std::string & key) const
{
    if (!m_pt.m_termKeys.count(key))
        throw std::out_of_range("Terminal does not exist");

    int idx = (m_pt.m_termKeys.size() * m_prodIndex) + m_pt.m_termKeys.at(key);
    return m_pt.m_table[idx];
}

ParseTable::ParseTable(const std::vector<TransformationTableEntry> &tte)
    : m_numProd(0), m_numTerm(0), m_table(nullptr)
{
    if (tte.size() == 0)
        throw std::runtime_error("Transformation table is empty");

    auto productions = productionSet(tte);
    auto terminals = terminalSet(tte);
    m_numProd = productions.size();
    m_numTerm = terminals.size();
    m_table = new std::string[m_numProd * m_numTerm];
    
    // map all the variables
    int i = 0;
    for (const auto &p : productions) {
        m_prodKeys[p] = i++;
    }

    // map all the terminals
    i = 0;
    for (const auto &t : terminals) {
        m_termKeys[t] = i++;
    }

    // create the parse table
    createTable(tte);
}

ParseTable::~ParseTable()
{
    delete[] m_table;
}

ParseTable::KeyProxy ParseTable::operator[](const std::string & key) const
{
    if (!m_prodKeys.count(key))
        throw std::out_of_range("Production does not exist");


    return ParseTable::KeyProxy(*this, m_prodKeys.at(key));
}

std::map<std::string, std::string> ParseTable::getTerminals(const std::string & key) const
{
    return std::map<std::string, std::string>();
}

std::set<std::string> ParseTable::terminalSet(const std::vector<TransformationTableEntry>& tte) const
{
    std::set<std::string> terminals;
    
    for (const auto &e : tte) {
        const auto &v = e.getFirstSet();

        for (const auto &t : v) {
            if (t != "''")
                terminals.insert(t);
        }

        auto &v = e.getFollowSet();

        for (const auto &t : v) {
            if (t != "''")
                terminals.insert(t);
        }
    }
    
    return terminals;
}

std::set<std::string> ParseTable::productionSet(const std::vector<TransformationTableEntry>& tte) const
{
    using namespace Util;

    std::set<std::string> productions;

    for (const auto &e : tte) {
        const auto &v = e.getProduction();
        // split the production rules
        auto tokens = String::split(v, " -> ");
        // only want the production
        productions.insert(tokens[0]);
    }

    return productions;
}

void ParseTable::createTable(const std::vector<TransformationTableEntry>& tte)
{
    using namespace Util;

    for (const auto &e : tte) {
        const auto &first = e.getFirstSet();
        const auto &follow = e.getFollowSet();
        // split on ' -> '
        auto prod = String::split(e.getProduction(), " -> ");

        // non-epsilon production
        if (prod[1] != "''") {
            for (const auto &f : first) {
                m_table[(m_prodKeys[prod[0]] * m_termKeys.size()) + m_termKeys[f]] = prod[1];
            }
        }
        else {
            for (const auto &f : follow) {
                m_table[(m_prodKeys[prod[0]] * m_termKeys.size()) + m_termKeys[f]] = prod[1];
            }
        }
    }
}

