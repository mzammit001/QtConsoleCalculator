#pragma once
#include "common.h"

/**
 * @brief LL(1) Parse Table for deriving productions
 */
class ParseTable
{
private:
    std::map<std::string, int> m_prodKeys;
    std::map<std::string, int> m_termKeys;

    std::string *m_table;

    int m_numProd;
    int m_numTerm;

public:
    ParseTable(int nProduction, int nTerminal);
    ~ParseTable();

    KeyProxy operator[](const std::string &key) const;
    std::map<std::string, std::string> getTerminals(const std::string &key) const;

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
};
