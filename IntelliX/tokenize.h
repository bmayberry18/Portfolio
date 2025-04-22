#ifndef TOKENIZE_H
#define TOKENIZE_H

#include <string>
#include <vector>
#pragma once

enum class TokenType
{
    INT_KEYWORD,
    FLOAT_KEYWORD,
    STRING_KEYWORD,
    IDENTIFIER,
    ASSIGN,
    NUMBER,
    STRING,
    SEMICOLON,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_QUOTE,
    RIGHT_QUOTE,
    PRINT_KEYWORD,
    BOOL_KEYWORD,
    BOOLEAN,
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    POWER,
    MODULO,
    ADDITION_ASSIGNMENT,
    SUBTRACTION_ASSIGNMENT,
    MULTIPLICATION_ASSIGNMENT,
    DIVISION_ASSIGNMENT,
    INT_CAST,
    FLOAT_CAST,
    STRING_CAST,
    BOOL_CAST,
    FUNCTION_DECLARATION,
    FUNCTION_RETURN_TYPE,
    FUNCTION_NAME,
    FUNCTION_BODY,
    LEFT_BRACE,
    RIGHT_BRACE,
    RETURN_TYPE,
    ARRAY_BRACKET, // Token type for array brackets
    LEFT_BRACKET,   // Token type for left bracket in array initialization
    RIGHT_BRACKET,  // Token type for right bracket in array initialization
    COMMA,           // Token type for comma in array initialization
    ARRAY_INDEX_VALUE,
    DOT,
    POP_METHOD,
    POP_BACK_METHOD,
    SUM_METHOD,
    PUSH_METHOD,
    PUSH_BACK_METHOD,
    MIN_METHOD,
    MAX_METHOD,
    REVERSE_METHOD,
    DELETE_DUPLICATES_METHOD,
    RANDOMIZE_METHOD,
    NULL_VALUE,    
    FOR_KEYWORD,
    FOR_INIT,
    FOR_CONDITION,
    FOR_INCREMENT,
    LESS_THAN,
    GREATER_THAN,
    INCREMENT,
    DECREMENT,
    FORMATTED_STRING,
    COMMENT,
    DOC_COMMENT,
    INPUT,
    INFILE,
    OUTFILE,
    FILE_KEYWORD,
    LESS_THAN_EQUAL,
    END_OF_FILE
};

struct Token
{
    TokenType type;
    std::string value;
};

std::string token_to_string(TokenType type);
 

std::vector<Token> tokenize(const std::string &input);

#endif // TOKENIZE_H