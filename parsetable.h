#pragma once
#include "common.h"

class TransformationTableEntry {
public:
    TransformationTableEntry(std::string& prod, std::vector<std::string>& first, std::vector<std::string>& follow);
    
    const std::string &getProduction() const;
    const std::vector<std::string> &getFirstSet() const;
    const std::vector<std::string> &getFollowSet() const;

private:
    std::string m_prod;
    std::vector<std::string> m_first;
    std::vector<std::string> m_follow;
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
    ParseTable(const std::vector<TransformationTableEntry> &tte);
    ~ParseTable();

    KeyProxy operator[](const std::string &key) const;
    std::map<std::string, std::string> getTerminals(const std::string &key) const;
    
private:
    std::set<std::string> terminalSet(const std::vector<TransformationTableEntry> &tte) const;
    std::set<std::string> productionSet(const std::vector<TransformationTableEntry> &tte) const;
    void createTable(const std::vector<TransformationTableEntry> &tte);

    std::map<std::string, int> m_prodKeys;
    std::map<std::string, int> m_termKeys;
    int m_numProd;
    int m_numTerm;
    std::string *m_table;
};
