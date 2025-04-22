#ifndef PARSE
#define PARSE
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include "tokenize.h"
#include "error_messages.h"
#include "array.h"

using namespace std;

struct Variable
{
    string type;
    string name;
    string value;
};

struct Function{
    string return_type;
    string name;
    vector<Token> parameters;
    vector<Token> body;
};

struct Loop{
    vector<Token> parameters;
    vector<Token> body;
};

extern vector<Variable> variables;
extern vector<Function> functions;

Variable findVariable(const string &name);

std::vector<std::string> parseArrayElements(const std::string &array_str);
void updateVariable(const string &name, const string &value, const string &type);

void parse(const std::vector<Token> &tokens);

#endif