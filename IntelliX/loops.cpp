#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <random>
#include "tokenize.h"
#include "error_messages.h"
#include "array.h"
#include "parse.h"
#include "loops.h"

using namespace std;

void parseCondition(const std::vector<Token> &tokens, size_t &pos, bool &conditionMet)
{   
    // Basic condition parsing logic
    if (tokens[pos].type == TokenType::IDENTIFIER || tokens[pos].type == TokenType::NUMBER)
    {
        double lhs;
        if (tokens[pos].type == TokenType::IDENTIFIER)
        {
            Variable lhs_var = findVariable(tokens[pos].value);
            lhs = stod(lhs_var.value);
        }
        else
        {
            lhs = stod(tokens[pos].value);
        }
        pos++;

        if (tokens[pos].type == TokenType::LESS_THAN || tokens[pos].type == TokenType::GREATER_THAN || tokens[pos].type == TokenType::LESS_THAN_EQUAL)
        {
            TokenType comparison = tokens[pos].type;
            pos++;

            double rhs;
            if (tokens[pos].type == TokenType::IDENTIFIER)
            {
                Variable rhs_var = findVariable(tokens[pos].value);
                rhs = stod(rhs_var.value);
            }
            else
            {
                rhs = stod(tokens[pos].value);
            }
            pos++;

            if (comparison == TokenType::LESS_THAN)
            {
                conditionMet = lhs < rhs;
            }
            else if (comparison == TokenType::GREATER_THAN)
            {
                conditionMet = lhs > rhs;
            }
            else if (comparison == TokenType::LESS_THAN_EQUAL)
            {
                conditionMet = lhs <= rhs;
            }
        }
    }
    else
    {
        Error("Invalid condition in for loop");
        throw std::runtime_error("Invalid condition in for loop");
    }
}

void parseIncrementDecrement(const std::vector<Token> &tokens, size_t &pos)
{
    // Basic increment/decrement parsing logic
    if (tokens[pos].type == TokenType::IDENTIFIER)
    {
        string var_name = tokens[pos].value;
        pos++;
        if (tokens[pos].type == TokenType::INCREMENT)
        {
            Variable variable = findVariable(var_name);
            int value = stoi(variable.value);
            value++;
            updateVariable(var_name, to_string(value), "int");
            pos++;
        }
        else if (tokens[pos].type == TokenType::DECREMENT)
        {
            Variable variable = findVariable(var_name);
            int value = stoi(variable.value);
            value--;
            updateVariable(var_name, to_string(value), "int");
            pos++;
        }
    }
    else
    {
        Error("Invalid increment/decrement in for loop");
        throw std::runtime_error("Invalid increment/decrement in for loop");
    }
}

void parseForLoop(const std::vector<Token> &tokens, size_t &pos)
{    

    if (tokens[pos].type == TokenType::FOR_KEYWORD)
    {
        pos++;
        if (tokens[pos].type == TokenType::LEFT_PAREN)
        {
            pos++;
            if (tokens[pos].type == TokenType::INT_KEYWORD)
            {
                // New variable initialization
                pos++;
                if (tokens[pos].type == TokenType::IDENTIFIER)
                {
                    std::string var_name = tokens[pos].value;
                    pos++;
                    if (tokens[pos].type == TokenType::ASSIGN)
                    {
                        pos++;
                        if (tokens[pos].type == TokenType::NUMBER)
                        {
                            std::string init_value = tokens[pos].value;
                            variables.push_back({"int", var_name, init_value});
                            pos++;
                            if (tokens[pos].type == TokenType::SEMICOLON)
                            {
                                pos++;
                                // Parse the condition
                                bool conditionMet = true;
                                parseCondition(tokens, pos, conditionMet);
                                if (tokens[pos].type == TokenType::SEMICOLON)
                                {
                                    pos++; 
                                    // Parse the increment/decrement
                                    size_t increment_start = pos;
                                    size_t body_end;
                                    while (conditionMet)
                                    {
                                        pos = increment_start;
                                        // Execute the loop body
                                        pos += 4; // Skip RIGHT_PAREN and LEFT_BRACE
                                        size_t body_start = pos;
                                        while (tokens[pos].type != TokenType::RIGHT_BRACE)
                                        {
                                            pos++;
                                        }
                                        std::vector<Token> body_tokens(tokens.begin() + body_start, tokens.begin() + pos);
                                        parse(body_tokens);
                                        body_end = pos;
                                        pos = increment_start;
                                        parseIncrementDecrement(tokens, pos);
                                        pos -= 6; // Move to the condition part
                                        parseCondition(tokens, pos, conditionMet);
                                    }
                                    pos = body_end;

                                    if (tokens[pos].type == TokenType::RIGHT_BRACE && tokens[pos + 1].type == TokenType::SEMICOLON)
                                    {
                                        pos += 2;
                                        return;
                                    }
                                    
                                }
                            }
                        }
                    }
                }
            }
            else if (tokens[pos].type == TokenType::IDENTIFIER)
            {
                // Existing variable usage
                std::string var_name = tokens[pos].value;
                pos++;
                if (tokens[pos].type == TokenType::SEMICOLON)
                {
                    pos++;
                    // Ensure variable is defined and of type int
                    Variable var = findVariable(var_name);
                    if (var.type != "int")
                    {
                        Error("Variable in for loop must be of type int");
                        throw std::runtime_error("Variable in for loop must be of type int");
                    }
                    // Parse the condition
                    bool conditionMet = true;
                    parseCondition(tokens, pos, conditionMet);
                    if (tokens[pos].type == TokenType::SEMICOLON)
                    {
                        pos++;
                        // Parse the increment/decrement
                        size_t increment_start = pos;
                        size_t body_end;
                        while (conditionMet)
                        {
                            pos = increment_start;
                            // Execute the loop body
                            pos += 4; // Skip RIGHT_PAREN and LEFT_BRACE
                            size_t body_start = pos;
                            while (tokens[pos].type != TokenType::RIGHT_BRACE)
                            {
                                pos++;
                            }
                            std::vector<Token> body_tokens(tokens.begin() + body_start, tokens.begin() + pos);
                            parse(body_tokens);
                            body_end = pos;
                            pos = increment_start;
                            parseIncrementDecrement(tokens, pos);
                            pos -= 6; // Move to the condition part
                            parseCondition(tokens, pos, conditionMet);
                        }
                        pos = body_end;
                        if (tokens[pos].type == TokenType::RIGHT_BRACE && tokens[pos + 1].type == TokenType::SEMICOLON)
                        {
                            pos += 2;
                            return;
                        }
                    }
                }
            }
        }
    }
    Error("Invalid for loop syntax");
    throw std::runtime_error("Invalid for loop syntax");
}