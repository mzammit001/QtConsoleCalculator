#pragma once
#include "common.h"

class ParseTableGenerator
{
    void firstSet();
    void followSet();
    bool isValid();

public:
    ParseTableGenerator(PairVector &grammar);
    ~ParseTableGenerator();

    bool getParseTable();
};

