#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <regex>
#include "tokenize.h"
#include "error_messages.h"
#include "array.h"
#include "parse.h"
#include "loops.h"
#include "math.h"

using namespace std;

vector<Variable> variables;
vector<Function> functions;
vector<Loop> loops;

Variable findVariable(const string &name)
{
    for (const auto &variable : variables)
    {
        if (variable.name == name)
        {
            return variable;
        }
    }
    Error("Variable not found: " + name);
    throw runtime_error("Variable not found: " + name);
}

void updateVariable(const string &name, const string &value, const string &type)
{
    auto it = find_if(variables.begin(), variables.end(), [&](const Variable &v)
                      { return v.name == name; });
    if (it != variables.end())
    {
        if (it->type == type)
        {
            it->value = value;
        }
        else if (it->type == "int")
        {
            if (type == "float")
            {
                double float_value = stod(value);
                int int_value = static_cast<int>(round(float_value));
                it->value = to_string(int_value);
                stringstream warning;
                warning << "Assigned Float {" << float_value << "} to " << it->name << " as {" << int_value << "}";
                Warning(warning.str());
            }
            else if (type == "bool")
            {
                bool bool_value = value == "true";
                it->value = bool_value ? "1" : "0";
            }
            else
            {
                it->value = value;
            }
        }
        else if (it->type == "float")
        {
            double float_value = stod(value);
            stringstream ss;
            ss << fixed << setprecision(10) << float_value;
            string float_str = ss.str();
            size_t dot_pos = float_str.find('.');
            if (dot_pos != string::npos)
            {
                while (float_str.back() == '0')
                {
                    float_str.pop_back();
                }
                if (float_str.back() == '.')
                {
                    float_str.pop_back();
                }
            }
            it->value = float_str;
        }
        else if (it->type == "bool")
        {
            bool bool_value = value == "true";
            it->value = bool_value ? "true" : "false";
        }

        else
        {
            stringstream error;
            error << "Cannot reassign a " << it->type << " variable with a " << type << " value";
            Error(error.str());
            throw runtime_error("Cannot reassign a " + it->type + " variable with a " + type + " value.");
        }
        cout << "Reassigned variable: " << it->type << "(" << it->name << ") = " << it->value << ";" << endl;
    }
    else
    {
        stringstream error;
        error << "Variable " << name << " does not exist.";
        Error(error.str());
        throw runtime_error("Variable " + name + " does not exist.");
    }
}

void parsePrintStatement(const vector<Token> &tokens, size_t &pos) {
    if (tokens[pos].type == TokenType::PRINT_KEYWORD) {
        pos++;

        if (tokens[pos].type == TokenType::LEFT_PAREN) {
            pos++;

            if (tokens[pos].type == TokenType::FORMATTED_STRING) {
                std::string formatted_string = tokens[pos].value;
                pos++;

                // Replace placeholders with variable values
                std::regex placeholder_pattern(R"(\{([a-zA-Z_]\w*)\})");
                std::smatch placeholder_match;
                while (std::regex_search(formatted_string, placeholder_match, placeholder_pattern)) {
                    std::string var_name = placeholder_match.str(1);
                    Variable variable = findVariable(var_name);
                    formatted_string.replace(placeholder_match.position(), placeholder_match.length(), variable.value);
                }

                if (tokens[pos].type == TokenType::RIGHT_PAREN) {
                    pos++;

                    if (tokens[pos].type == TokenType::SEMICOLON) {
                        cout << formatted_string << endl;
                        pos++;
                        return;
                    }
                }
            }
            else if (tokens[pos].type == TokenType::IDENTIFIER)
            {
                string var_name = tokens[pos].value;
                Variable variable = findVariable(var_name);
                pos++;

                if (tokens[pos].type == TokenType::RIGHT_PAREN)
                {
                    pos++;

                    if (tokens[pos].type == TokenType::SEMICOLON)
                    {
                        cout << variable.value << endl;
                        pos++;
                        return;
                    }
                }
            }
            else if (tokens[pos].type == TokenType::NUMBER)
            {
                double value = stod(tokens[pos].value);
                pos++;

                if (tokens[pos].type == TokenType::RIGHT_PAREN)
                {
                    pos++;

                    if (tokens[pos].type == TokenType::SEMICOLON)
                    {
                        cout << value << endl;
                        pos++;
                        return;
                    }
                }
            }
            else if (tokens[pos].type == TokenType::STRING)
            {
                string value = tokens[pos].value;
                pos++;

                if (tokens[pos].type == TokenType::RIGHT_PAREN)
                {
                    pos++;

                    if (tokens[pos].type == TokenType::SEMICOLON)
                    {
                        cout << value << endl;
                        pos++;
                        return;
                    }
                }
            }
        }
    }
    Error("Invalid print statement");
    throw runtime_error("Invalid print statement");
}

void parseInputStatement(const vector<Token> &tokens, size_t &pos) {
    if (tokens[pos].type == TokenType::INPUT) {
        pos++;

        if (tokens[pos].type == TokenType::LEFT_PAREN) {
            pos++;

            if (tokens[pos].type == TokenType::STRING) {
                // Handle input with a prompt string
                string prompt = tokens[pos].value;
                pos++;

                if (tokens[pos].type == TokenType::COMMA) {
                    pos++;

                    vector<string> var_names;
                    while (tokens[pos].type == TokenType::IDENTIFIER) {
                        var_names.push_back(tokens[pos].value);
                        pos++;

                        if (tokens[pos].type == TokenType::COMMA) {
                            pos++;
                        } else if (tokens[pos].type == TokenType::RIGHT_PAREN) {
                            break;
                        }
                    }

                    if (tokens[pos].type == TokenType::RIGHT_PAREN) {
                        pos++;

                        if (tokens[pos].type == TokenType::SEMICOLON) {
                            pos++;

                            // Output the prompt
                            cout << prompt << " ";

                            // Loop through all the variable names and get the inputs
                            for (const auto &var_name : var_names) {
                                string value;
                                cin >> value;

                                // Update the variable
                                Variable variable = findVariable(var_name);
                                updateVariable(var_name, value, variable.type);
                            }
                            return;
                        }
                    }
                }
            } else {
                // Handle input without a prompt string
                vector<string> var_names;
                while (tokens[pos].type == TokenType::IDENTIFIER) {
                    var_names.push_back(tokens[pos].value);
                    pos++;

                    if (tokens[pos].type == TokenType::COMMA) {
                        pos++;
                    } else if (tokens[pos].type == TokenType::RIGHT_PAREN) {
                        break;
                    }
                }

                if (tokens[pos].type == TokenType::RIGHT_PAREN) {
                    pos++;

                    if (tokens[pos].type == TokenType::SEMICOLON) {
                        pos++;

                        // Loop through all the variable names and get the inputs
                        for (const auto &var_name : var_names) {
                            string value;
                            cin >> value;

                            // Update the variable
                            Variable variable = findVariable(var_name);
                            updateVariable(var_name, value, variable.type);
                        }
                        return;
                    }
                }
            }
        }
    }
    Error("Invalid input statement");
    throw runtime_error("Invalid input statement");
}

void parseFileDeclaration(const vector<Token> &tokens, size_t &pos) {
    if (tokens[pos].type == TokenType::FILE_KEYWORD) {
        pos++;

        if (tokens[pos].type == TokenType::IDENTIFIER) {
            string var_name = tokens[pos].value;
            pos++;

            if (tokens[pos].type == TokenType::ASSIGN) {
                pos++;

                if (tokens[pos].type == TokenType::INFILE || tokens[pos].type == TokenType::OUTFILE) {
                    TokenType file_type = tokens[pos].type;
                    pos++;

                    if (tokens[pos].type == TokenType::LEFT_PAREN) {
                        pos++;
                        string file_name;
                        if (tokens[pos].type == TokenType::STRING) {
                            file_name = tokens[pos].value;
                            pos++;
                        }else if(tokens[pos].type == TokenType::IDENTIFIER){
                            Variable var = findVariable(tokens[pos].value);
                            file_name = var.value;
                            pos++;
                        }else{
                            Error("Incorrect Type for File Name");
                            throw runtime_error("Incorrect Type for File Name");
                        }

                        if (tokens[pos].type == TokenType::RIGHT_PAREN) {
                            pos++;

                                if (tokens[pos].type == TokenType::SEMICOLON) {
                                    pos++;

                                    // Handle the file declaration logic here
                                    if (file_type == TokenType::INFILE) {
                                        // Open the file and read its contents into the variable
                                        ifstream infile(file_name);
                                        if (!infile.is_open()) {
                                            Error("Failed to open file: " + file_name);
                                            throw runtime_error("Failed to open file: " + file_name);
                                        }

                                        variables.push_back({"infile", var_name, ""});

                                        stringstream buffer;
                                        buffer << infile.rdbuf();
                                        string file_contents = buffer.str();
                                        infile.close();

                                        // Update the variable with the file contents
                                        string content_name = var_name+"_content";
                                        variables.push_back({"string", content_name, file_contents});
                                    } else if (file_type == TokenType::OUTFILE) {
                                        // Find the variable and write its contents to the file


                                        ofstream outfile(file_name);
                                        if (!outfile.is_open()) {
                                            Error("Failed to open file: " + file_name);
                                            throw runtime_error("Failed to open file: " + file_name);
                                        }
                                        variables.push_back({"outfile", var_name, ""});

                                        outfile.close();
                                    }
                                    return;
                                }
                            }
                    }
                }
            }
        }
    }
    Error("Invalid file declaration");
    throw runtime_error("Invalid file declaration");
}

void parseInfileStatement(const vector<Token> &tokens, size_t &pos) {
    if (tokens[pos].type == TokenType::INFILE) {
        pos++;

        if (tokens[pos].type == TokenType::LEFT_PAREN) {
            pos++;

            if (tokens[pos].type == TokenType::IDENTIFIER) {
                string var_name = tokens[pos].value;
                pos++;

                if (tokens[pos].type == TokenType::RIGHT_PAREN) {
                    pos++;

                    if (tokens[pos].type == TokenType::SEMICOLON) {
                        pos++;

                        Variable variable = findVariable(var_name);
                        // Open the file and read its contents into the variable
                        ifstream infile(variable.value);
                        if (!infile.is_open()) {
                            Error("Failed to open file: " + variable.value);
                            throw runtime_error("Failed to open file: " + variable.value);
                        }

                        stringstream buffer;
                        buffer << infile.rdbuf();
                        string file_contents = buffer.str();
                        infile.close();

                        // Update the variable with the file contents
                        variables.push_back({"string", "infile", file_contents});
                        return;
                    }
                }
            }
        }
    }
    Error("Invalid infile statement");
    throw runtime_error("Invalid infile statement");
}

void parseOutfileStatement(const vector<Token> &tokens, size_t &pos) {
    if (tokens[pos].type == TokenType::OUTFILE) {
        pos++;

        if (tokens[pos].type == TokenType::LEFT_PAREN) {
            pos++;

            if (tokens[pos].type == TokenType::IDENTIFIER) {
                string var_name = tokens[pos].value;
                pos++;

                if (tokens[pos].type == TokenType::RIGHT_PAREN) {
                    pos++;

                    if (tokens[pos].type == TokenType::SEMICOLON) {
                        pos++;

                        // Find the variable and write its contents to the file
                        Variable variable = findVariable(var_name);
                        ofstream outfile(variable.value);
                        if (!outfile.is_open()) {
                            Error("Failed to open file: " + variable.value);
                            throw runtime_error("Failed to open file: " + variable.value);
                        }

                        outfile << variable.value;
                        outfile.close();
                        return;
                    }
                }
            }
        }
    }
    Error("Invalid outfile statement");
    throw runtime_error("Invalid outfile statement");
}


void handleIdentifierAssignment(const vector<Token> &tokens, size_t &pos, const string &var_name) {
    Variable variable = findVariable(tokens[pos].value);
    if (variable.type == "int") {
        string value_str = variable.value;
        pos++;
        if (tokens[pos].type == TokenType::SEMICOLON) {
            cout << "Parsed integer declaration: int " << var_name << " = " << value_str << ";" << endl;
            variables.push_back({"int", var_name, value_str});
            pos++;
        } else {
            Error("Invalid integer declaration");
            throw runtime_error("Invalid integer declaration");
        }
    } else {
        Error("Invalid integer declaration");
        throw runtime_error("Invalid integer declaration");
    }
}

void handleNumericAssignment(const vector<Token> &tokens, size_t &pos, const string &var_name) {
    double value = stod(tokens[pos].value);
    int rounded_value = static_cast<int>(round(value));
    string value_str = to_string(rounded_value);
    pos++;
    if (tokens[pos].type == TokenType::SEMICOLON) {
        if (value != rounded_value) {
            stringstream warning;
            warning << "Assigned Float {" << value << "} to " << var_name << " as {" << rounded_value << "}";
            Warning(warning.str());
        }
        cout << "Parsed integer declaration: int(" << var_name << ") = " << value_str << ";" << endl;
        variables.push_back({"int", var_name, value_str});
        pos++;
    } else {
        Error("Invalid integer declaration");
        throw runtime_error("Invalid integer declaration");
    }
}

void handleNullAssignment(const string &var_name) {
    cout << "Parsed integer declaration: int " << var_name << " = null;" << endl;
    variables.push_back({"int", var_name, "null"});
}

void parseIntegerDeclaration(const vector<Token> &tokens, size_t &pos) {
    if (tokens[pos].type != TokenType::IDENTIFIER) {
        Error("Invalid integer declaration");
        throw runtime_error("Invalid integer declaration");
    }

    string var_name = tokens[pos].value;
    pos++;

    if (tokens[pos].type == TokenType::ASSIGN) {
        pos++;
        if (tokens[pos].type == TokenType::IDENTIFIER) {
            handleIdentifierAssignment(tokens, pos, var_name);
        } else if (tokens[pos].type == TokenType::NUMBER) {
            handleNumericAssignment(tokens, pos, var_name);
        } else {
            Error("Invalid integer declaration");
            throw runtime_error("Invalid integer declaration");
        }
    } else if (tokens[pos].type == TokenType::SEMICOLON) {
        handleNullAssignment(var_name);
    } else {
        Error("Invalid integer declaration");
        throw runtime_error("Invalid integer declaration");
    }
}

void parseIntegerCast(const vector<Token> &tokens, size_t &pos){
    if(tokens[pos].type == TokenType::RIGHT_PAREN){
        pos++;
        if(tokens[pos].type == TokenType::IDENTIFIER){
            Variable variable = findVariable(tokens[pos].value);
            if (variable.type == "float")
            {
                int rounded_value = static_cast<int>(round(stod(variable.value)));
                Variable new_int = {"int", variable.name, to_string(rounded_value)};

                auto it = find_if(variables.begin(), variables.end(), [&variable](const Variable &v)
                                  { return v.name == variable.name; });

                if (it != variables.end())
                {
                    variables.erase(it);
                }

                variables.push_back(new_int);
            }
            else if (variable.type == "string")
            {
                int total = 0;
                for(char c : variable.value){
                    int ascii_value = static_cast<int>(c);
                    total += ascii_value;
                }

                Variable new_int = {"int", variable.name, to_string(total)};

                auto it = find_if(variables.begin(), variables.end(), [&variable](const Variable &v)
                                  { return v.name == variable.name; });

                if (it != variables.end())
                {
                    variables.erase(it);
                }

                variables.push_back(new_int);

            }

            pos++;

            if(tokens[pos].type == TokenType::SEMICOLON){
                pos++;
                return;
            }
            }
        }
        Error("Invalid integer cast");
    }

/*
This function parses a float declaration from the tokens. It checks for the
correct sequence of tokens for a float declaration and prints a message if the
declaration is valid. If the declaration is not valid, it throws a runtime error.
*/
 void parseFloatDeclaration(const vector<Token> &tokens, size_t &pos)
{
    if (tokens[pos].type == TokenType::IDENTIFIER)
    {
        string var_name = tokens[pos].value;
        pos++;

        if (tokens[pos].type == TokenType::ASSIGN)
        {
            pos++;
            if (tokens[pos].type == TokenType::IDENTIFIER)
            {
                Variable variable = findVariable(tokens[pos].value);
                if (variable.type == "float")
                {
                    string value_str = variable.value;
                    pos++;
                    if (tokens[pos].type == TokenType::SEMICOLON)
                    {
                        cout << "Parsed float declaration: float " << var_name << " = " << value_str << ";" << endl;
                        variables.push_back({"float", var_name, value_str});
                        pos++;
                        return;
                    }
                }
            }
            else if (tokens[pos].type == TokenType::NUMBER)
            {
                double value = stod(tokens[pos].value);
                stringstream ss;
                ss << fixed << setprecision(10) << value;
                string value_str = ss.str();
                size_t dot_pos = value_str.find('.');
                if (dot_pos != string::npos)
                {
                    while (value_str.back() == '0')
                    {
                        value_str.pop_back();
                    }
                    if (value_str.back() == '.')
                    {
                        value_str.pop_back();
                    }
                }
                pos++;
                if (tokens[pos].type == TokenType::SEMICOLON)
                {
                    cout << "Parsed float declaration: float " << var_name << " = " << value_str << ";" << endl;
                    variables.push_back({"float", var_name, value_str});
                    pos++;
                    return;
                }
            }
        }
        else if (tokens[pos].type == TokenType::SEMICOLON)
        {
            cout << "Parsed float declaration: float " << var_name << " = null;" << endl;
            variables.push_back({"float", var_name, "null"});
            pos++;
            return;
        }
    }
    Error("Invalid float declaration");
    throw runtime_error("Invalid float declaration");
}

    void parseFloatCast(const vector<Token> &tokens, size_t &pos)
    {
        if (tokens[pos].type == TokenType::RIGHT_PAREN)
        {
            pos++;
            if (tokens[pos].type == TokenType::IDENTIFIER)
            {
                Variable variable = findVariable(tokens[pos].value);
                if (variable.type == "int")
                {
                    float float_value = static_cast<float>(stoi(variable.value));
                    Variable new_float = {"float", variable.name, to_string(float_value)};

                    auto it = find_if(variables.begin(), variables.end(), [&variable](const Variable &v)
                                      { return v.name == variable.name; });

                    if (it != variables.end())
                    {
                        variables.erase(it);
                    }

                    variables.push_back(new_float);
                }
                else if (variable.type == "string")
                {
                    float total = 0.0f;
                    for (char c : variable.value)
                    {
                        float ascii_value = static_cast<float>(c);
                        total += ascii_value;
                    }

                    Variable new_float = {"float", variable.name, to_string(total)};

                    auto it = find_if(variables.begin(), variables.end(), [&variable](const Variable &v)
                                      { return v.name == variable.name; });

                    if (it != variables.end())
                    {
                        variables.erase(it);
                    }

                    variables.push_back(new_float);
                }

                pos++;

                if (tokens[pos].type == TokenType::SEMICOLON)
                {
                    pos++;
                    return;
                }
            }
        }
        Error("Invalid float cast");
        throw runtime_error("Invalid float cast");
    }
    

void parseStringDeclaration(const vector<Token> &tokens, size_t &pos)
{
    if (tokens[pos].type == TokenType::IDENTIFIER)
    {
        string var_name = tokens[pos].value;
        pos++;

        if (tokens[pos].type == TokenType::ASSIGN)
        {
            pos++;
            if (tokens[pos].type == TokenType::STRING)
            {
                string value = tokens[pos].value;
                pos++;

                if (tokens[pos].type == TokenType::SEMICOLON)
                {
                    cout << "Parsed string declaration: string " << var_name << " = " << value << ";" << endl;
                    variables.push_back({"string", var_name, value});
                    pos++;
                    return;
                }
            }
        }
        else if (tokens[pos].type == TokenType::SEMICOLON)
        {
            cout << "Parsed string declaration: string " << var_name << " = null;" << endl;
            variables.push_back({"string", var_name, "null"});
            pos++;
            return;
        }
    }
    Error("Invalid string declaration");
    throw runtime_error("Invalid string declaration");
}

void parseStringCast(const vector<Token> &tokens, size_t &pos)
{
    if (tokens[pos].type == TokenType::RIGHT_PAREN)
    {
        pos++;
        if (tokens[pos].type == TokenType::IDENTIFIER)
        {
            Variable variable = findVariable(tokens[pos].value);
            string new_string_value;

            if (variable.type == "int")
            {
                new_string_value = variable.value; // Direct conversion
            }
            else if (variable.type == "float")
            {
                new_string_value = variable.value; // Direct conversion
            }
            else if (variable.type == "string")
            {
                new_string_value = variable.value; // Direct conversion
            }
            else
            {
                Error("Unsupported type for string cast");
                throw runtime_error("Unsupported type for string cast");
            }

            Variable new_string = {"string", variable.name, new_string_value};

            auto it = find_if(variables.begin(), variables.end(), [&variable](const Variable &v)
                              { return v.name == variable.name; });

            if (it != variables.end())
            {
                variables.erase(it);
            }

            variables.push_back(new_string);

            pos++;

            if (tokens[pos].type == TokenType::SEMICOLON)
            {
                pos++;
                return;
            }
        }
    }
    Error("Invalid string cast");
    throw runtime_error("Invalid string cast");
}

void parseBooleanDeclaration(const vector<Token> &tokens, size_t &pos)
{
    if (tokens[pos].type == TokenType::IDENTIFIER)
    {
        string var_name = tokens[pos].value;
        pos++;

        // Check for optional parentheses
        if (tokens[pos].type == TokenType::LEFT_PAREN)
        {
            pos++;
            if (tokens[pos].type == TokenType::RIGHT_PAREN)
            {
                pos++;
            }
            else
            {
                Error("Expected right parenthesis");
                throw runtime_error("Expected right parenthesis");
            }
        }

        if (tokens[pos].type == TokenType::ASSIGN)
        {
            pos++;
            if (tokens[pos].type == TokenType::IDENTIFIER)
            {
                Variable variable = findVariable(tokens[pos].value);
                if (variable.type == "bool")
                {
                    string value_str = variable.value;
                    pos++;
                    if (tokens[pos].type == TokenType::SEMICOLON)
                    {
                        cout << "Parsed boolean declaration: bool " << var_name << " = " << value_str << ";" << endl;
                        variables.push_back({"bool", var_name, value_str});
                        pos++;
                        return;
                    }
                }
            }
            else if (tokens[pos].type == TokenType::BOOLEAN)
            {
                string value = tokens[pos].value;
                pos++;
                if (tokens[pos].type == TokenType::SEMICOLON)
                {
                    cout << "Parsed boolean declaration: bool " << var_name << " = " << value << ";" << endl;
                    variables.push_back({"bool", var_name, value});
                    pos++;
                    return;
                }
            }
        }
        else if (tokens[pos].type == TokenType::SEMICOLON)
        {
            cout << "Parsed boolean declaration: bool " << var_name << " = null;" << endl;
            variables.push_back({"bool", var_name, "null"});
            pos++;
            return;
        }
    }
    Error("Invalid boolean declaration");
    throw runtime_error("Invalid boolean declaration");
}

void parseBooleanCast(const vector<Token> &tokens, size_t &pos) {
    if (tokens[pos].type == TokenType::RIGHT_PAREN) {
        pos++;
        if (tokens[pos].type == TokenType::IDENTIFIER) {
            Variable variable = findVariable(tokens[pos].value);
            bool bool_value;

            if (variable.type == "float") {
                bool_value = (stod(variable.value) != 0.0);
            } else if (variable.type == "int") {
                bool_value = (stoi(variable.value) != 0);
            } else if (variable.type == "string") {
                bool_value = !variable.value.empty();
            } else {
                Error("Unsupported type for boolean cast");
                throw runtime_error("Unsupported type for boolean cast");
            }

            Variable new_bool = {"bool", variable.name, bool_value ? "true" : "false"};

            auto it = find_if(variables.begin(), variables.end(), [&variable](const Variable &v) {
                return v.name == variable.name;
            });

            if (it != variables.end()) {
                variables.erase(it);
            }

            variables.push_back(new_bool);

            pos++;

            if (tokens[pos].type == TokenType::SEMICOLON) {
                pos++;
                return;
            }
        }
    }
    Error("Invalid boolean cast");
    throw runtime_error("Invalid boolean cast");
}

// Update the parseIdentifierDeclaration function
void parseIdentifierDeclaration(const vector<Token> &tokens, size_t &pos)
{
    if (tokens[pos].type == TokenType::IDENTIFIER)
    {
        string var_name = tokens[pos].value;
        pos++;

        if (tokens[pos].type == TokenType::ASSIGN)
        {
            pos++;

            if (tokens[pos].type == TokenType::NUMBER)
            {
                float value = stof(tokens[pos].value);
                pos++;
                if (tokens[pos].type == TokenType::SEMICOLON)
                {
                    updateVariable(var_name, to_string(value), "int");
                    pos++;
                    return;
                }
            }
            else if (tokens[pos].type == TokenType::IDENTIFIER)
            {
                Variable variable = findVariable(tokens[pos].value);
                if (variable.type == "int")
                {
                    pos++;
                    if (tokens[pos].type == TokenType::SEMICOLON)
                    {
                        updateVariable(var_name, variable.value, "int");
                        pos++;
                        return;
                    }
                }
            }
            else if (tokens[pos].type == TokenType::STRING)
            {
                string value = tokens[pos].value;
                pos++;
                if (tokens[pos].type == TokenType::SEMICOLON)
                {
                    updateVariable(var_name, value, "string");
                    pos++;
                    return;
                }
            }
            else if (tokens[pos].type == TokenType::BOOLEAN)
            {
                string bool_value = tokens[pos].value;
                if (bool_value == "true")
                {
                    updateVariable(var_name, "true", "bool");
                }
                else if (bool_value == "false")
                {
                    updateVariable(var_name, "false", "bool");
                }
                else
                {
                    Error("Invalid boolean value");
                    throw runtime_error("Invalid boolean value");
                }
                pos++;
                if (tokens[pos].type == TokenType::SEMICOLON)
                {
                    pos++;
                    return;
                }
            }
        }
    }
    Error("Invalid Variable declaration");
    throw runtime_error("Invalid Variable declaration");
}


void parseFunctionDeclaration(const std::vector<Token> &tokens, size_t &pos)
{
    if (tokens[pos].type == TokenType::RETURN_TYPE)
    {
        std::string return_type = tokens[pos].value;
        pos++;

        if (tokens[pos].type == TokenType::FUNCTION_NAME)
        {
            std::string function_name = tokens[pos].value;
            pos++;

            if (tokens[pos].type == TokenType::LEFT_PAREN)
            {
                pos++;
                std::vector<Token> parameters;

                while (tokens[pos].type != TokenType::RIGHT_PAREN)
                {
                    parameters.push_back(tokens[pos]);
                    pos++;
                }

                pos++; // Skip the RIGHT_PAREN

                if (tokens[pos].type == TokenType::LEFT_BRACE)
                {
                    pos++;
                    std::vector<Token> body;

                    while (tokens[pos].type != TokenType::RIGHT_BRACE)
                    {
                        body.push_back(tokens[pos]);
                        pos++;
                    }

                    pos++; // Skip the RIGHT_BRACE

                    functions.push_back({return_type, function_name, parameters, body});
                    return;
                }
            }
        }
    }
    Error("Invalid function declaration");
    throw std::runtime_error("Invalid function declaration");
}



/*
This function takes the tokens and calls the appropriate parsing functions based
on the token types. It checks for the correct sequence of tokens for each
declaration and prints a message if the declaration is valid. If the declaration
is not valid, it throws a runtime error.
*/
void parse(const std::vector<Token> &tokens)
{
    size_t pos = 0;

    while (pos < tokens.size())
    {
        try
        {
            switch (tokens[pos].type)
            {
            case TokenType::INT_KEYWORD:
                pos++;
                if (tokens[pos].type == TokenType::ARRAY_BRACKET)
                {
                    parseArrayDeclaration(tokens, pos);
                }
                else
                {
                    parseIntegerDeclaration(tokens, pos);
                }
                break;
            case TokenType::FLOAT_KEYWORD:
                pos++;
                if (tokens[pos].type == TokenType::ARRAY_BRACKET)
                {
                    parseArrayDeclaration(tokens, pos);
                }
                else
                {
                    parseFloatDeclaration(tokens, pos);
                }
                break;
            case TokenType::STRING_KEYWORD:
                pos++;
                if (tokens[pos].type == TokenType::ARRAY_BRACKET)
                {
                    parseArrayDeclaration(tokens, pos);
                }
                else
                {
                    parseStringDeclaration(tokens, pos);
                }
                break;
            case TokenType::BOOL_KEYWORD:
                pos++;
                if (tokens[pos].type == TokenType::ARRAY_BRACKET)
                {
                    parseArrayDeclaration(tokens, pos);
                }
                else
                {
                    parseBooleanDeclaration(tokens, pos);
                }
                break;
            case TokenType::IDENTIFIER:
                pos++;
                if(tokens[pos].type == TokenType::DOT){
                    pos++;
                    if(tokens[pos].type == TokenType::POP_METHOD){
                        parseArrayPop(tokens, pos-=2);
                    }else if(tokens[pos].type == TokenType::POP_BACK_METHOD){
                        parseArrayPopBack(tokens, pos-=2);
                    }else if (tokens[pos].type == TokenType::SUM_METHOD)
                    {
                        parseArraySum(tokens, pos -= 2);
                    }else if (tokens[pos].type == TokenType::PUSH_METHOD)
                    {
                        parseArrayPush(tokens, pos -= 2);
                    }else if (tokens[pos].type == TokenType::PUSH_BACK_METHOD)
                    {
                        parseArrayPushBack(tokens, pos -= 2);
                    }else if (tokens[pos].type == TokenType::MIN_METHOD)
                    {
                        parseArrayMin(tokens, pos -= 2);
                    }else if (tokens[pos].type == TokenType::MAX_METHOD)
                    {
                        parseArrayMax(tokens, pos -= 2);
                    }else if (tokens[pos].type == TokenType::REVERSE_METHOD)
                    {
                        parseArrayReverse(tokens, pos -= 2);
                    }else if (tokens[pos].type == TokenType::DELETE_DUPLICATES_METHOD)
                    {
                        parseArrayDeleteDuplicates(tokens, pos -= 2);
                    }else if (tokens[pos].type == TokenType::RANDOMIZE_METHOD)
                    {
                        parseArrayRandomize(tokens, pos -= 2);
                    }

                }else if(tokens[pos].type == TokenType::LEFT_BRACKET){
                    parseArrayIndexing(tokens, pos-=1);
                }else if(tokens[pos].type == TokenType::ADDITION_ASSIGNMENT || tokens[pos].type == TokenType::SUBTRACTION_ASSIGNMENT ||
                         tokens[pos].type == TokenType::MULTIPLICATION_ASSIGNMENT || tokens[pos].type == TokenType::DIVISION_ASSIGNMENT){
                    parseAssignmentOperation(tokens, pos-=1);
                }else if (tokens[pos].type == TokenType::ASSIGN){
                    parseIdentifierDeclaration(tokens, pos-=2);
                }
                break;
            case TokenType::END_OF_FILE:
                return;
            case TokenType::PRINT_KEYWORD:
                parsePrintStatement(tokens, pos);
                break;
            case TokenType::INPUT:
                parseInputStatement(tokens, pos);
                break;
            case TokenType::RETURN_TYPE:
                parseFunctionDeclaration(tokens, pos);
                break;
            case TokenType::LEFT_PAREN:
                //cout<<"Read in left paren"<<endl;
                pos++;

                break;
            case TokenType::RIGHT_PAREN:
                //cout<<"Read in right paren"<<endl;
                pos++;
                break;
            case TokenType::POP_METHOD:
                pos++;
                break;
            case TokenType::FOR_KEYWORD:
                parseForLoop(tokens, pos);
                break;
            case TokenType::ADDITION:
            case TokenType::SUBTRACTION:
            case TokenType::MULTIPLICATION:
            case TokenType::DIVISION:
            case TokenType::POWER:
            case TokenType::MODULO:
                parseExpression(tokens, pos-=1);
                break;
            case TokenType::NUMBER:
                pos++;
                break;
            case TokenType::STRING:
                pos++;
                break;
            case TokenType::INFILE:
                parseInfileStatement(tokens, pos);
                break;
            case TokenType::OUTFILE:
                parseOutfileStatement(tokens, pos);
                break;
            case TokenType::FILE_KEYWORD:
                parseFileDeclaration(tokens, pos);
                break;
            case TokenType::SEMICOLON:
                pos++;
                break;
            default:
                // for(Token t: tokens){
                //     cout<<token_to_string(t.type)<<endl;
                // }

                cout<<"Error token: "<<token_to_string(tokens[pos].type)<<endl;
                Error("Invalid declaration");
                throw std::runtime_error("Invalid declaration");
            }
        }
        catch (const std::exception &e)
        {
            Error(e.what());
            return;
        }
    }
}
/*
This is the entry point of the program. It reads the input file, tokenizes the
input, and then parses the tokens. If there is an error during tokenization or
parsing, it prints an error message.
*/
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        stringstream ss;
        ss << "Usage: " << argv[0] << " <filename>";
        Error(ss.str());
        return 1;
    }

    ifstream file(argv[1]);
    if (!file.is_open())
    {
        Error("Failed to open file");
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string input = buffer.str();
    file.close();

    try
    {
        vector<Token> tokens = tokenize(input);
        for(Token t : tokens){
            std::cout << "{" << token_to_string(t.type) << "," << t.value << "}" << std::endl;
        }
        parse(tokens);

        for(Variable v: variables){
            cout<<"{"<<v.name<<", "<<v.type<<", "<<v.value<<"}\n";
        }

        for(Function f: functions){
            cout<<"{"<<f.return_type<<", "<<f.name;
            for(Token t: f.parameters){
                cout<<", "<<token_to_string(t.type);
            }
            for(Token t: f.body){
                cout<<", "<<token_to_string(t.type);
            }
            cout<<"}"<<endl;

        }
    }
    catch (const exception &e)
    {
        Error(e.what());
        return 1;
    }

    return 0;
}
