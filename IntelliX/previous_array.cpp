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


void parseArrayDeclaration(const std::vector<Token> &tokens, size_t &pos)
{
    TokenType type = tokens[pos - 1].type;
    if (tokens[pos].type == TokenType::ARRAY_BRACKET)
    {
        pos++;
        if (tokens[pos].type == TokenType::IDENTIFIER)
        {
            std::string var_name = tokens[pos].value;
            pos++;
            if (tokens[pos].type == TokenType::ASSIGN)
            {
                pos++;
                if (tokens[pos].type == TokenType::LEFT_BRACKET)
                {
                    pos++;
                    std::vector<std::string> elements;
                    while (tokens[pos].type != TokenType::RIGHT_BRACKET)
                    {
                        if (tokens[pos].type == TokenType::NUMBER)
                        {
                            elements.push_back(tokens[pos].value);
                        }
                        else if (tokens[pos].type == TokenType::STRING)
                        {
                            elements.push_back(tokens[pos].value);
                        }
                        else if (tokens[pos].type == TokenType::BOOLEAN)
                        {
                            elements.push_back(tokens[pos].value);
                        }
                        else
                        {
                            Error("Invalid array element");
                            throw std::runtime_error("Invalid array element");
                        }
                        pos++;
                        if (tokens[pos].type == TokenType::COMMA)
                        {
                            pos++;
                        }
                    }
                    pos++; // Skip the RIGHT_BRACKET
                    if (tokens[pos].type == TokenType::SEMICOLON)
                    {
                        pos++;
                        std::string type_str;
                        switch (type)
                        {
                        case TokenType::INT_KEYWORD:
                            type_str = "int";
                            break;
                        case TokenType::FLOAT_KEYWORD:
                            type_str = "float";
                            break;
                        case TokenType::STRING_KEYWORD:
                            type_str = "string";
                            break;
                        case TokenType::BOOL_KEYWORD:
                            type_str = "bool";
                            break;
                        default:
                            Error("Invalid array type");
                            throw std::runtime_error("Invalid array type");
                        }
                        std::cout << "Parsed array declaration with initialization: " << type_str << "[] " << var_name << " = [" << elements[0];
                        for (size_t i = 1; i < elements.size(); i++)
                        {
                            std::cout << ", " << elements[i];
                        }
                        std::cout << "];" << std::endl;
                        return;
                    }
                }
            }
            else if (tokens[pos].type == TokenType::SEMICOLON)
            {
                pos++;
                std::string type_str;
                switch (type)
                {
                case TokenType::INT_KEYWORD:
                    type_str = "int";
                    break;
                case TokenType::FLOAT_KEYWORD:
                    type_str = "float";
                    break;
                case TokenType::STRING_KEYWORD:
                    type_str = "string";
                    break;
                case TokenType::BOOL_KEYWORD:
                    type_str = "bool";
                    break;
                default:
                    Error("Invalid array type");
                    throw std::runtime_error("Invalid array type");
                }
                std::cout << "Parsed array declaration: " << type_str << "[] " << var_name << ";" << std::endl;
                return;
            }
        }
    }
    Error("Invalid array declaration");
    throw std::runtime_error("Invalid array declaration");
}