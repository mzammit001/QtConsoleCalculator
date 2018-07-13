#pragma once
#include "common.h"

class TransformationTableEntry {
public:
    TransformationTableEntry(StringPair& prod, StringVector& first, StringVector& follow);
    
    const StringPair &getProduction() const;
    const StringVector &getFirstSet() const;
    const StringVector &getFollowSet() const;

private:
    StringPair m_prod;
    StringVector m_first;
    StringVector m_follow;
};

/**
 * @brief LL(1) Parse Table for deriving productions
 */
class ParseTable
{
public:
    /**
    * @brief Proxy for x[][] syntax
    */
    class KeyProxy {
    private:
        const ParseTable &m_pt;
        int m_prodIndex;

    public:
        KeyProxy(const ParseTable &pt, int prodIndex);
        std::string operator[](const std::string &key) const;
    };

public:
    ParseTable(std::string &start, const std::vector<TransformationTableEntry> &tte);
    ~ParseTable();

    std::string getStartVar();

    KeyProxy operator[](const std::string &key) const;
    std::map<std::string, std::string> getTerminals(const std::string &key) const;
    
private:
    std::set<std::string> terminalSet(const std::vector<TransformationTableEntry> &tte) const;
    std::set<std::string> productionSet(const std::vector<TransformationTableEntry> &tte) const;
    void createTable(const std::vector<TransformationTableEntry> &tte);

private:
    std::string m_start;
    std::map<std::string, int> m_prodKeys;
    std::map<std::string, int> m_termKeys;
    size_t m_numProd;
    size_t m_numTerm;
    std::string *m_table;
};
