#include "tokenize.h"
#include <regex>
#include <iostream>
#include <stdexcept>
#include "error_messages.h"

std::vector<Token> tokenize(const std::string &input)
{
    std::vector<Token> tokens;
    std::regex int_pattern(R"(\bint\b)");
    std::regex float_pattern(R"(\bfloat\b)");
    std::regex string_pattern(R"(\bstring\b)");
    std::regex identifier_pattern(R"([a-zA-Z_]\w*)");
    std::regex assign_pattern(R"(=)");
    std::regex number_pattern(R"(-?\d+(\.\d+)?)");
    std::regex string_literal_pattern(R"(([""][^"]*["]|[']?[^']+\'?))");
    std::regex semicolon_pattern(R"(;)");
    std::regex left_paren_pattern(R"(\()");
    std::regex right_paren_pattern(R"(\))");
    std::regex print_pattern(R"(\bprint\b)");
    std::regex bool_pattern(R"(\bbool\b)");
    std::regex boolean_pattern(R"(true|false)");
    std::regex math_operation_pattern(R"(\+|-|\*|/)");
    std::regex function_decl_pattern(R"((void|int|float|string|bool)\s+[a-zA-Z_]\w*\s*\([^)]*\)\s*\{[^}]*\})");
    std::regex array_bracket_pattern(R"(\[\])"); // Pattern for array brackets
    std::regex left_bracket_pattern(R"(\[)");    // Pattern for left bracket in array initialization
    std::regex right_bracket_pattern(R"(\])");   // Pattern for right bracket in array initialization
    std::regex comma_pattern(R"(,)");             // Pattern for comma in array initialization


    size_t pos = 0;
    while (pos < input.size())
    {
        if (isspace(input[pos]))
        {
            pos++;
            continue;
        }

        std::smatch match;
        std::string_view input_view(input.c_str() + pos, input.size() - pos);

        if (std::regex_search(input.begin() + pos, input.end(), match, function_decl_pattern) && match.position() == 0)
        {
            std::string function_decl = match.str();
            pos += match.length();

            // Further tokenize the function declaration
            std::regex return_type_pattern(R"((void|int|float|string|bool))");
            std::regex function_name_pattern(R"([a-zA-Z_]\w*)");
            std::regex parameters_pattern(R"(\([^)]*\))");
            std::regex body_pattern(R"(\{[^}]*\})");

            std::smatch return_type_match, function_name_match, parameters_match, body_match;
            std::string return_type, function_name, parameters, body;

            // Match return type
            if (std::regex_search(function_decl, return_type_match, return_type_pattern))
            {
                return_type = return_type_match.str();
                tokens.push_back({TokenType::RETURN_TYPE, return_type});
                function_decl = function_decl.substr(return_type_match.length()); // Remove matched portion
            }

            // Match function name
            if (std::regex_search(function_decl, function_name_match, function_name_pattern))
            {
                function_name = function_name_match.str();
                tokens.push_back({TokenType::FUNCTION_NAME, function_name});
                function_decl = function_decl.substr(function_name_match.position() + function_name_match.length()); // Remove matched portion
            }

            // Match parameters
            if (std::regex_search(function_decl, parameters_match, parameters_pattern))
            {
                parameters = parameters_match.str();
                tokens.push_back({TokenType::LEFT_PAREN, "("});
                std::string params_content = parameters.substr(1, parameters.size() - 2);
                if (!params_content.empty())
                {
                    std::vector<Token> params_tokens = tokenize(params_content);
                    tokens.insert(tokens.end(), params_tokens.begin(), params_tokens.end());
                }
                tokens.push_back({TokenType::RIGHT_PAREN, ")"});
                function_decl = function_decl.substr(parameters_match.position() + parameters_match.length()); // Remove matched portion
            }

            // Match body
            if (std::regex_search(function_decl, body_match, body_pattern))
            {
                body = body_match.str();
                tokens.push_back({TokenType::LEFT_BRACE, "{"});
                std::string body_content = body.substr(1, body.size() - 2);
                if (!body_content.empty())
                {
                    std::vector<Token> body_tokens = tokenize(body_content);
                    tokens.insert(tokens.end(), body_tokens.begin(), body_tokens.end());
                }
                tokens.push_back({TokenType::RIGHT_BRACE, "}"});
            }

            continue;
        }


        if (std::regex_search(input.begin() + pos, input.end(), match, print_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::PRINT_KEYWORD, "print"});
            pos += match.length();
            continue; 
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, int_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::INT_KEYWORD, "int"});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, float_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::FLOAT_KEYWORD, "float"});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, string_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::STRING_KEYWORD, "string"});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, bool_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::BOOL_KEYWORD, "bool"});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, boolean_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::BOOLEAN, match.str()});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, identifier_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::IDENTIFIER, match.str()});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, assign_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::ASSIGN, "="});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, number_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::NUMBER, match.str()});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, semicolon_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::SEMICOLON, ";"});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, left_paren_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::LEFT_PAREN, "("});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, right_paren_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::RIGHT_PAREN, ")"});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, math_operation_pattern) && match.position() == 0)
        {
            tokens.push_back({static_cast<TokenType>(match.str()[0]), match.str()});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, array_bracket_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::ARRAY_BRACKET, "[]"});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, left_bracket_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::LEFT_BRACKET, "["});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, right_bracket_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::RIGHT_BRACKET, "]"});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, comma_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::COMMA, ","});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, string_literal_pattern) && match.position() == 0)
        {
            std::string literal = match.str();
            if (literal.front() == '\'' || literal.front() == '\"')
            {
                literal = literal.substr(1, literal.size() - 2);
            }
            tokens.push_back({TokenType::STRING, literal});
            pos += literal.size() + 2;
            continue;
        }



        throw std::runtime_error("Unexpected character: " + std::string(1, input[pos]));
    }

    if (!tokens.empty() && tokens.back().type == TokenType::SEMICOLON)
    {
        tokens.push_back({TokenType::END_OF_FILE, ""});
    }
    else
    {
        throw std::runtime_error("Unexpected end of file");
    }

    //for (Token t : tokens)
    //{
    //    std::cout << "{" << token_to_string(t.type) << "," << t.value << "}" << std::endl;
    //}



    return tokens;
}

std::string token_to_string(TokenType type)
{
    switch (type)
    {
    case TokenType::INT_KEYWORD:
        return "INT_KEYWORD";
    case TokenType::INT_CAST:
        return "INT_CAST";
    case TokenType::FLOAT_KEYWORD:
        return "FLOAT_KEYWORD";
    case TokenType::FLOAT_CAST:
        return "FLOAT_CAST";
    case TokenType::STRING_KEYWORD:
        return "STRING_KEYWORD";
    case TokenType::STRING_CAST:
        return "STRING_CAST";
    case TokenType::IDENTIFIER:
        return "IDENTIFIER";
    case TokenType::ASSIGN:
        return "ASSIGN";
    case TokenType::NUMBER:
        return "NUMBER";
    case TokenType::STRING:
        return "STRING";
    case TokenType::SEMICOLON:
        return "SEMICOLON";
    case TokenType::LEFT_PAREN:
        return "LEFT_PAREN";
    case TokenType::RIGHT_PAREN:
        return "RIGHT_PAREN";
    case TokenType::LEFT_BRACE:
        return "LEFT_BRACE";
    case TokenType::RIGHT_BRACE:
        return "RIGHT_BRACE";
    case TokenType::PRINT_KEYWORD:
        return "PRINT_KEYWORD";
    case TokenType::BOOL_KEYWORD:
        return "BOOL_KEYWORD";
    case TokenType::BOOL_CAST:
        return "BOOL_CAST";
    case TokenType::BOOLEAN:
        return "BOOLEAN";
    case TokenType::FUNCTION_DECLARATION:
        return "FUNCTION_DECLARATION";
    case TokenType::FUNCTION_NAME:
        return "FUNCTION_NAME";
    case TokenType::RETURN_TYPE:
        return "RETURN_TYPE";
    case TokenType::END_OF_FILE:
        return "END_OF_FILE";
    // Add other cases as needed
    default:
        return "UNKNOWN";
    }
};