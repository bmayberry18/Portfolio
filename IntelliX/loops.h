#ifndef LOOPS_H
#define LOOPS_H

#include <vector>
#include <iostream>
#include <cmath>
#include "tokenize.h"
#include "parse.h"

using namespace std;

void parseForLoop(const std::vector<Token> &tokens, size_t &pos);
void parseCondition(const std::vector<Token> &tokens, size_t &pos, bool &conditionMet);
void parseIncrementDecrement(const std::vector<Token> &tokens, size_t &pos);

#endif