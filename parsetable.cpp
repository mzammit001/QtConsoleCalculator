#include "parsetable.h"

TransformationTableEntry::TransformationTableEntry(const StringPair & prod, const StringVector & first, const StringVector & follow)
    : m_prod(prod), m_first(first), m_follow(follow)
{
}

const StringPair &TransformationTableEntry::getProduction() const
{
    return m_prod;
}

const StringVector &TransformationTableEntry::getFirstSet() const
{
    return m_first;
}

const StringVector &TransformationTableEntry::getFollowSet() const
{
    return m_follow;
}

ParseTable::KeyProxy::KeyProxy(const ParseTable & pt, int prodIndex)
    : m_pt(pt), m_prodIndex(prodIndex)
{
}

ParseTable::KeyProxy::KeyProxy(ParseTable & pt, int prodIndex)
    : m_pt(pt), m_prodIndex(prodIndex)
{
}

const std::string& ParseTable::KeyProxy::operator[](const std::string & key) const
{
    if (!m_pt.m_termKeys.count(key))
        throw std::out_of_range("Terminal does not exist");

    size_t idx = (m_pt.m_termKeys.size() * m_prodIndex) + m_pt.m_termKeys.at(key);
    return m_pt.m_table[idx];
}

std::string & ParseTable::KeyProxy::operator[](const std::string & key)
{
    if (!m_pt.m_termKeys.count(key))
        throw std::out_of_range("Terminal does not exist");

    size_t idx = (m_pt.m_termKeys.size() * m_prodIndex) + m_pt.m_termKeys.at(key);
    return m_pt.m_table[idx];
}

ParseTable::ParseTable(std::string &start, const std::vector<TransformationTableEntry> &tte)
    : m_start(start), m_numProd(0), m_numTerm(0), m_table(nullptr)
{
    if (tte.size() == 0)
        throw std::runtime_error("Transformation table is empty");

    auto productions = productionSet(tte);
    auto terminals = terminalSet(tte);
    m_numProd = productions.size();
    m_numTerm = terminals.size();
    m_table = new std::string[m_numProd * m_numTerm]();
    
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

std::string ParseTable::getStartVar()
{
    return m_start;
}

const ParseTable::KeyProxy ParseTable::operator[](const std::string & key) const
{
    if (!m_prodKeys.count(key))
        throw std::out_of_range("Production does not exist");


    return ParseTable::KeyProxy(*this, m_prodKeys.at(key));
}

ParseTable::KeyProxy ParseTable::operator[](const std::string & key)
{
    if (!m_prodKeys.count(key))
        throw std::out_of_range("Production does not exist");


    return ParseTable::KeyProxy(*this, m_prodKeys.at(key));
}

std::set<std::string> ParseTable::getTerminals(const std::string & key) const
{
    std::set<std::string> terms;
    
    for (auto it = m_termKeys.begin(); it != m_termKeys.end(); ++it) {
        const std::pair<std::string, int> &entry = *it;

        const ParseTable &pt = *this;
        const std::string &value = pt[key][entry.first];
        if (value.size() != 0) {
            terms.insert(entry.first);
        }
    }

    return terms;
}

std::set<std::string> ParseTable::terminalSet(const std::vector<TransformationTableEntry>& tte) const
{
    std::set<std::string> terminals;
    
    for (const auto &e : tte) {
        const auto &first = e.getFirstSet();

        for (const auto &t : first) {
            if (t != NullSymbol)
                terminals.insert(t);
        }

        const auto &follow = e.getFollowSet();

        for (const auto &t : follow) {
            if (t != NullSymbol)
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
        // only want the production
        productions.insert(v.first);
    }

    return productions;
}

void ParseTable::createTable(const std::vector<TransformationTableEntry>& tte)
{
    using namespace Util;
    ParseTable &pt = *this;

    for (const auto &e : tte) {
        const auto &first = e.getFirstSet();
        const auto &follow = e.getFollowSet();
        // split on ' -> '
        auto prod = e.getProduction();

        // non-epsilon production
        if (prod.second != NullSymbol) {
            for (const auto &f : first) {
                pt[prod.first][f] = prod.second;
                std::cout << "pt[" << prod.first << "][" << f << "]: " << pt[prod.first][f] << std::endl;
            }
        }
        else {
            for (const auto &f : follow) {
                pt[prod.first][f] = prod.second;
                std::cout << "pt[" << prod.first << "][" << f << "]: " << pt[prod.first][f] <<std::endl;
            }
        }
    }
}

