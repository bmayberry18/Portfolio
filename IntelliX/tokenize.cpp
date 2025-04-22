#include "tokenize.h"
#include <regex>
#include <iostream>
#include <stdexcept>
#include "error_messages.h"

using namespace std;

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
    std::regex function_decl_pattern(R"((void|int|float|string|bool)\s+[a-zA-Z_]\w*\s*\([^)]*\)\s*\{[^}]*\})");
    std::regex array_bracket_pattern(R"(\[\])");
    std::regex left_bracket_pattern(R"(\[)");
    std::regex right_bracket_pattern(R"(\])");
    std::regex comma_pattern(R"(,)");
    std::regex array_index_value_pattern(R"(\d+)");
    std::regex array_pop_pattern(R"(([a-zA-Z_]\w*)\.pop\(([^)]*)\)\s*;)");
    std::regex array_pop_back_pattern(R"(([a-zA-Z_]\w*)\.pop_back\(\)\s*;)");  // New pattern for 'array'.pop_back();
    std::regex array_sum_pattern(R"(([a-zA-Z_]\w*)\.sum\(\)\s*;)");  // New pattern for 'array'.sum();
    std::regex array_push_pattern(R"(([a-zA-Z_]\w*)\.push\(([^)]*)\)\s*;)");  // New pattern for 'array'.push(element);
    std::regex array_push_back_pattern(R"(([a-zA-Z_]\w*)\.push_back\(([^)]*)\)\s*;)");
    std::regex array_min_pattern(R"(([a-zA-Z_]\w*)\.min\(\)\s*;)");  // New pattern for 'array'.min();
    std::regex array_max_pattern(R"(([a-zA-Z_]\w*)\.max\(\)\s*;)");
    std::regex array_reverse_pattern(R"(([a-zA-Z_]\w*)\.reverse\(\)\s*;)");
    std::regex array_delete_duplicates_pattern(R"(([a-zA-Z_]\w*)\.delete_duplicates\(\)\s*;)");
    std::regex array_randomize_pattern(R"(([a-zA-Z_]\w*)\.randomize\(\)\s*;)");
    std::regex for_loop_with_init_condition_increment_pattern(R"(for\s*\(\s*int\s+([a-zA-Z_]\w*)\s*=\s*(-?\d+)\s*;\s*([^;]+)\s*;\s*([^;]+)\s*\)\s*\{\s*([^}]*)\s*\}\s*;)");
    std::regex for_loop_existing_var_with_condition_increment_pattern(R"(for\s*\(\s*([a-zA-Z_]\w*)\s*;\s*([^;]+)\s*;\s*([^;]+)\s*\)\s*\{\s*([^}]*)\s*\}\s*;)");
    std::regex less_than_pattern(R"(<)");
    std::regex greater_than_pattern(R"(>)");
    std::regex increment_pattern(R"(\+\+)");
    std::regex decrement_pattern(R"(\-\-)");
    std::regex formatted_string_pattern(R"(f\"(.*?)\")");
    std::regex addition_pattern(R"(\+)"); // New pattern for addition
    std::regex subtraction_pattern(R"(-)"); // New pattern for subtraction
    std::regex multiplication_pattern(R"(\*)"); // New pattern for multiplication
    std::regex division_pattern(R"(/)"); // New pattern for division
    std::regex power_pattern(R"(\^)"); // New pattern for power
    std::regex modulo_pattern(R"(%)"); // New pattern for modulo
    std::regex addition_assignment_pattern(R"(\+=)"); // Pattern for +=
    std::regex subtraction_assignment_pattern(R"(-=)"); // Pattern for -=
    std::regex multiplication_assignment_pattern(R"(\*=)"); // Pattern for *=
    std::regex division_assignment_pattern(R"(/=)"); // Pattern for /=
    std::regex identifier_addition_pattern(R"(([a-zA-Z_]\w*)\s*\+\s*(-?\d+(\.\d+)?)\s*;)"); // New pattern for identifier + number;
    std::regex comment_pattern(R"(//.*?$)", std::regex::multiline); // New pattern for comments
    std::regex multi_line_comment_pattern(R"(/\*[\s\S]*?\*/)"); // Pattern for multi-line comments
    std::regex doc_comment_pattern(R"(@DOC:\s*(Function|Variable):\s*([a-zA-Z_]\w*)\s*([\s\S]*?)@END)"); // Updated pattern for DOC_COMMENT
    std::regex input_pattern(R"(\binput\s*\(\s*([a-zA-Z_]\w*)(\s*,\s*[a-zA-Z_]\w*)*\s*\)\s*;)");
    std::regex input_with_prompt_pattern(R"(\binput\s*\(\s*\"([^\"]*)\"\s*,\s*([a-zA-Z_]\w*)(\s*,\s*[a-zA-Z_]\w*)*\s*\)\s*;)");
    std::regex infile_pattern(R"(\binfile\s*\(\s*([a-zA-Z_]\w*)\s*\)\s*;)");
    std::regex outfile_pattern(R"(\boutfile\s*\(\s*([a-zA-Z_]\w*)\s*\)\s*;)");
    std::regex file_pattern(R"(\bFile\b)");
    regex less_than_equals_pattern(R"(<=)");


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

        // Check for doc comment pattern
        if (std::regex_search(input.begin() + pos, input.end(), match, doc_comment_pattern) && match.position() == 0)
        {
            std::string type = match.str(1); // Either "Function" or "Variable"
            std::string name = match.str(2);
            std::string comment_body = match.str(3);
            //tokens.push_back({TokenType::DOC_COMMENT, "@DOC: " + type + ": " + name + " " + comment_body + "@END"});
            pos += match.length();
            continue;
        }

        // Check for comment pattern
        if (std::regex_search(input.begin() + pos, input.end(), match, comment_pattern) && match.position() == 0)
        {
            pos += match.length();
            continue;
        }

        // Check for file pattern
        if (std::regex_search(input.begin() + pos, input.end(), match, file_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::FILE_KEYWORD, "File"});
            pos += match.length();
            continue;
        }

        // Check for multi-line comment pattern
        if (std::regex_search(input.begin() + pos, input.end(), match, multi_line_comment_pattern) && match.position() == 0)
        {
            pos += match.length();
            continue;
        }

        // Check for infile pattern
        if (std::regex_search(input.begin() + pos, input.end(), match, infile_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::INFILE, "infile"});
            tokens.push_back({TokenType::LEFT_PAREN, "("});
            tokens.push_back({TokenType::IDENTIFIER, match.str(1)});
            tokens.push_back({TokenType::RIGHT_PAREN, ")"});
            tokens.push_back({TokenType::SEMICOLON, ";"});
            pos += match.length();
            continue;
        }

        // Check for outfile pattern
        if (std::regex_search(input.begin() + pos, input.end(), match, outfile_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::OUTFILE, "outfile"});
            tokens.push_back({TokenType::LEFT_PAREN, "("});
            tokens.push_back({TokenType::IDENTIFIER, match.str(1)});
            tokens.push_back({TokenType::RIGHT_PAREN, ")"});
            tokens.push_back({TokenType::SEMICOLON, ";"});
            pos += match.length();
            continue;
        }

        // Check for identifier + number; pattern
        if (std::regex_search(input.begin() + pos, input.end(), match, identifier_addition_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::IDENTIFIER, match.str(1)});
            tokens.push_back({TokenType::ADDITION, "+"});
            tokens.push_back({TokenType::NUMBER, match.str(2)});
            tokens.push_back({TokenType::SEMICOLON, ";"});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, for_loop_with_init_condition_increment_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::FOR_KEYWORD, "for"});
            tokens.push_back({TokenType::LEFT_PAREN, "("});
            tokens.push_back({TokenType::INT_KEYWORD, "int"});

            // Capture the variable name
            std::string var_name = match.str(1);
            tokens.push_back({TokenType::IDENTIFIER, var_name});

            tokens.push_back({TokenType::ASSIGN, "="});

            // Capture the initialization value
            std::string init_value = match.str(2);
            tokens.push_back({TokenType::NUMBER, init_value});

            tokens.push_back({TokenType::SEMICOLON, ";"});

            // Capture and tokenize the condition
            std::string condition = match.str(3) + ";";
            std::vector<Token> condition_tokens = tokenize(condition);
            condition_tokens.erase(
                std::remove_if(
                    condition_tokens.begin(),
                    condition_tokens.end(),
                    [](const Token& token) { return token.type == TokenType::SEMICOLON || token.type == TokenType::END_OF_FILE; }
                ),
                condition_tokens.end()
            );
            tokens.insert(tokens.end(), condition_tokens.begin(), condition_tokens.end());

            tokens.push_back({TokenType::SEMICOLON, ";"});

            // Capture and tokenize the increment/decrement
            std::string increment = match.str(4) + ";";
            std::vector<Token> increment_tokens = tokenize(increment);
            increment_tokens.erase(
                std::remove_if(
                    increment_tokens.begin(),
                    increment_tokens.end(),
                    [](const Token& token) { return token.type == TokenType::SEMICOLON || token.type == TokenType::END_OF_FILE; }
                ),
                increment_tokens.end()
            );
            tokens.insert(tokens.end(), increment_tokens.begin(), increment_tokens.end());

            tokens.push_back({TokenType::RIGHT_PAREN, ")"});
            tokens.push_back({TokenType::LEFT_BRACE, "{"});

            // Capture and tokenize the loop body
            std::string body = match.str(5);
            
            std::vector<Token> body_tokens = tokenize(body);
            body_tokens.erase(
                std::remove_if(
                    body_tokens.begin(),
                    body_tokens.end(),
                    [](const Token& token) { return token.type == TokenType::END_OF_FILE; }
                ),
                body_tokens.end()
            );            
            tokens.insert(tokens.end(), body_tokens.begin(), body_tokens.end());

            tokens.push_back({TokenType::RIGHT_BRACE, "}"});
            tokens.push_back({TokenType::SEMICOLON, ";"});

            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, for_loop_existing_var_with_condition_increment_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::FOR_KEYWORD, "for"});
            tokens.push_back({TokenType::LEFT_PAREN, "("});

            // Capture the variable name
            std::string var_name = match.str(1);
            tokens.push_back({TokenType::IDENTIFIER, var_name});

            tokens.push_back({TokenType::SEMICOLON, ";"});

            // Capture and tokenize the condition
            std::string condition = match.str(2) + ";";
            std::vector<Token> condition_tokens = tokenize(condition);
            condition_tokens.erase(
                std::remove_if(
                    condition_tokens.begin(),
                    condition_tokens.end(),
                    [](const Token& token) { return token.type == TokenType::SEMICOLON || token.type == TokenType::END_OF_FILE; }
                ),
                condition_tokens.end()
            );
            tokens.insert(tokens.end(), condition_tokens.begin(), condition_tokens.end());

            tokens.push_back({TokenType::SEMICOLON, ";"});

            // Capture and tokenize the increment/decrement
            std::string increment = match.str(3) + ";";
            std::vector<Token> increment_tokens = tokenize(increment);
            increment_tokens.erase(
                std::remove_if(
                    increment_tokens.begin(),
                    increment_tokens.end(),
                    [](const Token& token) { return token.type == TokenType::SEMICOLON || token.type == TokenType::END_OF_FILE; }
                ),
                increment_tokens.end()
            );
            tokens.insert(tokens.end(), increment_tokens.begin(), increment_tokens.end());

            tokens.push_back({TokenType::RIGHT_PAREN, ")"});
            tokens.push_back({TokenType::LEFT_BRACE, "{"});

            // Capture and tokenize the loop body
            std::string body = match.str(4);
            std::vector<Token> body_tokens = tokenize(body);
            body_tokens.erase(
                std::remove_if(
                    body_tokens.begin(),
                    body_tokens.end(),
                    [](const Token& token) {return token.type == TokenType::END_OF_FILE; }
                ),
                body_tokens.end()
            );  
            tokens.insert(tokens.end(), body_tokens.begin(), body_tokens.end());

            tokens.push_back({TokenType::RIGHT_BRACE, "}"});
            tokens.push_back({TokenType::SEMICOLON, ";"});

            pos += match.length();
            continue;
        }


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

        // Check for array pop pattern with element
        if (std::regex_search(input.begin() + pos, input.end(), match, array_pop_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::IDENTIFIER, match.str(1)});
            tokens.push_back({TokenType::DOT, "."});
            tokens.push_back({TokenType::POP_METHOD, "pop"});
            tokens.push_back({TokenType::LEFT_PAREN, "("});

            // Tokenize the element inside the parentheses
            std::string element = match.str(2);
            if(!element.empty()) {
                element += ";";
                std::vector<Token> element_tokens = tokenize(element);
                element_tokens.erase(
                    std::remove_if(
                        element_tokens.begin(),
                        element_tokens.end(),
                        [](const Token& token) { return token.type == TokenType::SEMICOLON || token.type == TokenType::END_OF_FILE; }
                    ),
                    element_tokens.end()
                );

                tokens.insert(tokens.end(), element_tokens.begin(), element_tokens.end());
            }else{
                tokens.push_back({TokenType::NULL_VALUE, "NULL"});
            }
            tokens.push_back({TokenType::RIGHT_PAREN, ")"});
            tokens.push_back({TokenType::SEMICOLON, ";"});
            pos += match.length();
            continue;
        }
        // Check for array pop_back pattern
        if (std::regex_search(input.begin() + pos, input.end(), match, array_pop_back_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::IDENTIFIER, match.str(1)});
            tokens.push_back({TokenType::DOT, "."});
            tokens.push_back({TokenType::POP_BACK_METHOD, "pop_back"});
            tokens.push_back({TokenType::LEFT_PAREN, "("});
            tokens.push_back({TokenType::RIGHT_PAREN, ")"});
            tokens.push_back({TokenType::SEMICOLON, ";"});
            pos += match.length();
            continue;
        }

        // Check for array sum pattern
        if (std::regex_search(input.begin() + pos, input.end(), match, array_sum_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::IDENTIFIER, match.str(1)});
            tokens.push_back({TokenType::DOT, "."});
            tokens.push_back({TokenType::SUM_METHOD, "sum"});
            tokens.push_back({TokenType::LEFT_PAREN, "("});
            tokens.push_back({TokenType::RIGHT_PAREN, ")"});
            tokens.push_back({TokenType::SEMICOLON, ";"});
            pos += match.length();
            continue;
        }

        // Check for array push pattern
        if (std::regex_search(input.begin() + pos, input.end(), match, array_push_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::IDENTIFIER, match.str(1)});
            tokens.push_back({TokenType::DOT, "."});
            tokens.push_back({TokenType::PUSH_METHOD, "push"});
            tokens.push_back({TokenType::LEFT_PAREN, "("});

            // Tokenize the element inside the parentheses
            std::string element = match.str(2) + ";";
            std::vector<Token> element_tokens = tokenize(element);
            element_tokens.erase(
                std::remove_if(
                    element_tokens.begin(),
                    element_tokens.end(),
                    [](const Token& token) { return token.type == TokenType::SEMICOLON || token.type == TokenType::END_OF_FILE; }
                ),
                element_tokens.end()
            );

            tokens.insert(tokens.end(), element_tokens.begin(), element_tokens.end());


            tokens.push_back({TokenType::RIGHT_PAREN, ")"});
            tokens.push_back({TokenType::SEMICOLON, ";"});
            pos += match.length();
            continue;
        }

        // Check for array push_back pattern
        if (std::regex_search(input.begin() + pos, input.end(), match, array_push_back_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::IDENTIFIER, match.str(1)});
            tokens.push_back({TokenType::DOT, "."});
            tokens.push_back({TokenType::PUSH_BACK_METHOD, "push_back"});
            tokens.push_back({TokenType::LEFT_PAREN, "("});

            // Tokenize the element inside the parentheses
            std::string element = match.str(2) + ";";
            std::vector<Token> element_tokens = tokenize(element);
            element_tokens.erase(
                std::remove_if(
                    element_tokens.begin(),
                    element_tokens.end(),
                    [](const Token& token) { return token.type == TokenType::SEMICOLON || token.type == TokenType::END_OF_FILE; }
                ),
                element_tokens.end()
            );

            tokens.insert(tokens.end(), element_tokens.begin(), element_tokens.end());

            tokens.push_back({TokenType::RIGHT_PAREN, ")"});
            tokens.push_back({TokenType::SEMICOLON, ";"});
            pos += match.length();
            continue;
        }

        // Check for array min pattern
        if (std::regex_search(input.begin() + pos, input.end(), match, array_min_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::IDENTIFIER, match.str(1)});
            tokens.push_back({TokenType::DOT, "."});
            tokens.push_back({TokenType::MIN_METHOD, "min"});
            tokens.push_back({TokenType::LEFT_PAREN, "("});
            tokens.push_back({TokenType::RIGHT_PAREN, ")"});
            tokens.push_back({TokenType::SEMICOLON, ";"});
            pos += match.length();
            continue;
        }

        // Check for array max pattern
        if (std::regex_search(input.begin() + pos, input.end(), match, array_max_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::IDENTIFIER, match.str(1)});
            tokens.push_back({TokenType::DOT, "."});
            tokens.push_back({TokenType::MAX_METHOD, "max"});
            tokens.push_back({TokenType::LEFT_PAREN, "("});
            tokens.push_back({TokenType::RIGHT_PAREN, ")"});
            tokens.push_back({TokenType::SEMICOLON, ";"});
            pos += match.length();
            continue;
        }

        // Check for array reverse pattern
        if (std::regex_search(input.begin() + pos, input.end(), match, array_reverse_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::IDENTIFIER, match.str(1)});
            tokens.push_back({TokenType::DOT, "."});
            tokens.push_back({TokenType::REVERSE_METHOD, "reverse"});
            tokens.push_back({TokenType::LEFT_PAREN, "("});
            tokens.push_back({TokenType::RIGHT_PAREN, ")"});
            tokens.push_back({TokenType::SEMICOLON, ";"});
            pos += match.length();
            continue;
        }

        // Check for array delete_duplicates pattern
        if (std::regex_search(input.begin() + pos, input.end(), match, array_delete_duplicates_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::IDENTIFIER, match.str(1)});
            tokens.push_back({TokenType::DOT, "."});
            tokens.push_back({TokenType::DELETE_DUPLICATES_METHOD, "delete_duplicates"});
            tokens.push_back({TokenType::LEFT_PAREN, "("});
            tokens.push_back({TokenType::RIGHT_PAREN, ")"});
            tokens.push_back({TokenType::SEMICOLON, ";"});
            pos += match.length();
            continue;
        }

        // Check for array randomize pattern
        if (std::regex_search(input.begin() + pos, input.end(), match, array_randomize_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::IDENTIFIER, match.str(1)});
            tokens.push_back({TokenType::DOT, "."});
            tokens.push_back({TokenType::RANDOMIZE_METHOD, "randomize"});
            tokens.push_back({TokenType::LEFT_PAREN, "("});
            tokens.push_back({TokenType::RIGHT_PAREN, ")"});
            tokens.push_back({TokenType::SEMICOLON, ";"});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, print_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::PRINT_KEYWORD, "print"});
            pos += match.length();
            continue;
        }

        // Check for input pattern with variables
        if (std::regex_search(input.begin() + pos, input.end(), match, input_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::INPUT, "input"});
            tokens.push_back({TokenType::LEFT_PAREN, "("});

            std::string input_string = match.str();
            size_t start = input_string.find('(') + 1;
            size_t _end = input_string.find(')');
            std::string variables_string = input_string.substr(start, _end - start);

            std::regex var_pattern(R"([a-zA-Z_]\w*)");
            std::sregex_iterator it(variables_string.begin(), variables_string.end(), var_pattern);
            std::sregex_iterator end;
            for (; it != end; ++it)
            {
                tokens.push_back({TokenType::IDENTIFIER, it->str()});
                if (std::next(it) != end)
                {
                    tokens.push_back({TokenType::COMMA, ","});
                }
            }

            tokens.push_back({TokenType::RIGHT_PAREN, ")"});
            tokens.push_back({TokenType::SEMICOLON, ";"});
            pos += match.length();
            continue;
        }

        // Check for input with prompt pattern
        if (std::regex_search(input.begin() + pos, input.end(), match, input_with_prompt_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::INPUT, "input"});
            tokens.push_back({TokenType::LEFT_PAREN, "("});
            tokens.push_back({TokenType::STRING, match.str(1)});
            tokens.push_back({TokenType::COMMA, ","});

            std::string input_string = match.str();
            size_t start = input_string.find(',') + 1;
            size_t _end = input_string.find(')');
            std::string variables_string = input_string.substr(start, _end - start);

            std::regex var_pattern(R"([a-zA-Z_]\w*)");
            std::sregex_iterator it(variables_string.begin(), variables_string.end(), var_pattern);
            std::sregex_iterator end;
            for (; it != end; ++it)
            {
                tokens.push_back({TokenType::IDENTIFIER, it->str()});
                if (std::next(it) != end)
                {
                    tokens.push_back({TokenType::COMMA, ","});
                }
            }

            tokens.push_back({TokenType::RIGHT_PAREN, ")"});
            tokens.push_back({TokenType::SEMICOLON, ";"});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, formatted_string_pattern) && match.position() == 0) {
            std::string formatted_string = match.str(1);
            tokens.push_back({TokenType::FORMATTED_STRING, formatted_string});
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

        if (std::regex_search(input.begin() + pos, input.end(), match, increment_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::INCREMENT, "++"});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, decrement_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::DECREMENT, "--"});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, addition_assignment_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::ADDITION_ASSIGNMENT, "+="});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, subtraction_assignment_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::SUBTRACTION_ASSIGNMENT, "-="});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, multiplication_assignment_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::MULTIPLICATION_ASSIGNMENT, "*="});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, division_assignment_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::DIVISION_ASSIGNMENT, "/="});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, addition_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::ADDITION, "+"});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, subtraction_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::SUBTRACTION, "-"});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, multiplication_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::MULTIPLICATION, "*"});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, division_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::DIVISION, "/"});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, power_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::POWER, "^"});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, modulo_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::MODULO, "%"});
            pos += match.length();
            continue;
        }

        if(regex_search(input.begin() + pos, input.end(), match, less_than_equals_pattern) && match.position()==0){
            tokens.push_back({TokenType::LESS_THAN_EQUAL, "<="});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, less_than_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::LESS_THAN, "<"});
            pos += match.length();
            continue;
        }

        if (std::regex_search(input.begin() + pos, input.end(), match, greater_than_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::GREATER_THAN, ">"});
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

        if (std::regex_search(input.begin() + pos, input.end(), match, array_index_value_pattern) && match.position() == 0)
        {
            tokens.push_back({TokenType::ARRAY_INDEX_VALUE, match.str()});
            pos += match.length();
            continue;
        }

        throw std::runtime_error("Unexpected character: " + std::string(1, input[pos]));
    }


    // for(Token t : tokens){
    //     cout<<token_to_string(t.type)<<endl;
    // }

    if (!tokens.empty() /*&& tokens.back().type == TokenType::SEMICOLON*/)
    {
        tokens.push_back({TokenType::END_OF_FILE, ""});
    }
    else
    {
        cout<<token_to_string(tokens[pos].type)<< tokens[pos].value<<endl;
        throw std::runtime_error("Unexpected end of file");
    }

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
    case TokenType::FILE_KEYWORD: // New case for FILE_KEYWORD
        return "FILE_KEYWORD";
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
    case TokenType::ARRAY_BRACKET:
        return "ARRAY_BRACKET";
    case TokenType::LEFT_BRACKET:
        return "LEFT_BRACKET";
    case TokenType::RIGHT_BRACKET:
        return "RIGHT_BRACKET";
    case TokenType::COMMA:
        return "COMMA";
    case TokenType::DOT:
        return "DOT";
    case TokenType::POP_METHOD:
        return "POP_METHOD";
    case TokenType::POP_BACK_METHOD:
        return "POP_BACK_METHOD";
    case TokenType::SUM_METHOD:
        return "SUM_METHOD";
    case TokenType::PUSH_METHOD:
        return "PUSH_METHOD";
    case TokenType::PUSH_BACK_METHOD:
        return "PUSH_BACK_METHOD";
    case TokenType::MIN_METHOD:
        return "MIN_METHOD";
    case TokenType::MAX_METHOD:
        return "MAX_METHOD";
    case TokenType::REVERSE_METHOD:
        return "REVERSE_METHOD";
    case TokenType::DELETE_DUPLICATES_METHOD:
        return "DELETE_DUPLICATES_METHOD";
    case TokenType::RANDOMIZE_METHOD:
        return "RANDOMIZE_METHOD";
    case TokenType::NULL_VALUE:
        return "NULL_VALUE";
    case TokenType::FOR_KEYWORD:
        return "FOR_KEYWORD";
    case TokenType::LESS_THAN:
        return "LESS_THAN";
    case TokenType::GREATER_THAN:
        return "GREATER_THAN";
    case TokenType::INCREMENT:
        return "INCREMENT";
    case TokenType::DECREMENT:
        return "DECREMENT";
    case TokenType::ADDITION:
        return "ADDITION";
    case TokenType::SUBTRACTION:
        return "SUBTRACTION";
    case TokenType::MULTIPLICATION:
        return "MULTIPLICATION";
    case TokenType::DIVISION:
        return "DIVISION";
    case TokenType::POWER:
        return "POWER";
    case TokenType::MODULO:
        return "MODULO";
    case TokenType::ADDITION_ASSIGNMENT:
        return "ADDITION_ASSIGNMENT";
    case TokenType::SUBTRACTION_ASSIGNMENT:
        return "SUBTRACTION_ASSIGNMENT";
    case TokenType::MULTIPLICATION_ASSIGNMENT:
        return "MULTIPLICATION_ASSIGNMENT";
    case TokenType::DIVISION_ASSIGNMENT:
        return "DIVISION_ASSIGNMENT";
    case TokenType::COMMENT:
        return "COMMENT"; // New case for COMMENT
    case TokenType::DOC_COMMENT: // New case for DOC_COMMENT
        return "DOC_COMMENT";
    case TokenType::INPUT:
        return "INPUT";
    case TokenType::INFILE:
        return "INFILE";
    case TokenType::OUTFILE: // New case for OUTFILE
        return "OUTFILE";
    // Add other cases as needed
    default:
        return "UNKNOWN";
    }
};