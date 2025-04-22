#ifndef MATH_H
#define MATH_H

#include <vector>
#include <iostream>
#include <cmath>
#include "tokenize.h"
#include "parse.h"

using namespace std;

string parseAdditionExpression(const std::vector<Token> &tokens, size_t &pos);
string parseSubtractionExpression(const std::vector<Token>& tokens, size_t& pos);
string parseAdditionSubtractionExpression(const std::vector<Token>& tokens, size_t& pos);
string parseExpression(const std::vector<Token>& tokens, size_t& pos);
void parseAssignmentOperation(const std::vector<Token>& tokens, size_t& pos);

#endif