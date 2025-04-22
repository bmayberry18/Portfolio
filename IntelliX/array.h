#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include <iostream>
#include <cmath>
#include "tokenize.h"
#include "parse.h"

using namespace std;

void parseArrayDeclaration(const std::vector<Token> &tokens, size_t &pos);
void parseArrayIndexing(const std::vector<Token> &tokens, size_t &pos);
void parseArrayPop(const std::vector<Token> &tokens, size_t &pos);
string parseArrayPopBack(const std::vector<Token> &tokens, size_t &pos);
void parseArraySum(const vector<Token> &tokens, size_t &pos);
void parseArrayPush(const vector<Token> &tokens, size_t &pos);
void parseArrayPushBack(const vector<Token> &tokens, size_t &pos);
void parseArrayMin(const vector<Token> &tokens, size_t &pos);
void parseArrayMax(const vector<Token> &tokens, size_t &pos);
void parseArrayReverse(const vector<Token> &tokens, size_t &pos);
void parseArrayDeleteDuplicates(const vector<Token> &tokens, size_t &pos);
void parseArrayRandomize(const vector<Token> &tokens, size_t &pos);



#endif

