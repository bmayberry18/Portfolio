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

void parseArrayDeclaration(const vector<Token> &tokens, size_t &pos)
{
    TokenType type = tokens[pos - 1].type;
    if (tokens[pos].type == TokenType::ARRAY_BRACKET)
    {
        pos++;
        if (tokens[pos].type == TokenType::IDENTIFIER)
        {
            string var_name = tokens[pos].value;
            pos++;
            if (tokens[pos].type == TokenType::ASSIGN)
            {
                pos++;
                if (tokens[pos].type == TokenType::LEFT_BRACKET)
                {
                    pos++;
                    vector<string> elements;
                    while (tokens[pos].type != TokenType::RIGHT_BRACKET)
                    {
                        if (tokens[pos].type == TokenType::NUMBER ||
                            tokens[pos].type == TokenType::STRING ||
                            tokens[pos].type == TokenType::BOOLEAN)
                        {
                            elements.push_back(tokens[pos].value);
                        }
                        else
                        {
                            Error("Invalid array element");
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
                        string type_str;
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
                        }

                        // Store elements as a single string representation
                        ostringstream oss;
                        oss << "[";
                        for (size_t i = 0; i < elements.size(); ++i)
                        {
                            if (i > 0) oss << ",";
                            oss << elements[i];
                        }
                        oss << "]";
                        string array_value = oss.str();

                        // Add the array to the variables
                        variables.push_back({type_str + "[]", var_name, array_value});

                        cout << "Parsed array declaration with initialization: " << type_str << "[] " << var_name << " = " << array_value << ";" << ::endl;
                        return;
                    }
                }
            }
            else if (tokens[pos].type == TokenType::SEMICOLON)
            {
                pos++;
                string type_str;
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
                }

                // Add an empty array to the variables
                variables.push_back({type_str + "[]", var_name, "[]"});

                cout << "Parsed array declaration: " << type_str << "[] " << var_name << ";" << ::endl;
                return;
            }
        }
    }
    Error("Invalid array declaration");
}

void parseArrayIndexing(const vector<Token> &tokens, size_t &pos)
{
    if (tokens[pos].type == TokenType::IDENTIFIER)
    {
        string var_name = tokens[pos].value;
        pos++;

        if (tokens[pos].type == TokenType::LEFT_BRACKET)
        {
            pos++;
            if (tokens[pos].type == TokenType::NUMBER)
            {
                int index = stoi(tokens[pos].value);
                pos++;
                if (tokens[pos].type == TokenType::RIGHT_BRACKET)
                {
                    pos++;

                    // Check if it's an assignment
                    if (tokens[pos].type == TokenType::ASSIGN)
                    {
                        pos++;
                        if (tokens[pos].type == TokenType::NUMBER)
                        {
                            string new_value = tokens[pos].value;
                            pos++;
                            if (tokens[pos].type == TokenType::SEMICOLON)
                            {
                                // Update the array value
                                auto it = find_if(variables.begin(), variables.end(), [&](const Variable &v) {
                                    return v.name == var_name;
                                });

                                if (it != variables.end())
                                {
                                    vector<string> elements = parseArrayElements(it->value);
                                    if (index >= 0 && index < elements.size())
                                    {
                                        elements[index] = new_value;

                                        // Update the array string
                                        ostringstream oss;
                                        oss << "[";
                                        for (size_t i = 0; i < elements.size(); ++i)
                                        {
                                            if (i > 0) oss << ",";
                                            oss << elements[i];
                                        }
                                        oss << "]";
                                        it->value = oss.str();

                                        cout << "Updated array indexing assignment: " << var_name << "[" << index << "] = " << new_value << ";" << endl;
                                    }
                                    else
                                    {
                                        Error("Array index out of bounds");
                                    }
                                }
                                else
                                {
                                    Error("Array not found");
                                }
                                pos++;
                                return;
                            }
                        }
                    }
                    else if (tokens[pos].type == TokenType::SEMICOLON)
                    {
                        // Retrieval logic, if needed
                        Variable array_var = findVariable(var_name);
                        vector<string> array_elements = parseArrayElements(array_var.value);
                        
                        if (index >= 0 && index < array_elements.size())
                        {
                            cout << "Array element value: " << array_elements[index] << endl;
                        }
                        else
                        {
                            Error("Array index out of bounds");
                        }
                        pos++;
                        return;
                    }
                }
            }
        }
    }
    Error("Invalid array indexing");
}

void parseArrayPop(const vector<Token> &tokens, size_t &pos)
{
    if (tokens[pos].type == TokenType::IDENTIFIER)
    {
        string var_name = tokens[pos].value;
        pos++;

        if (tokens[pos].type == TokenType::DOT)
        {
            pos++;

            if (tokens[pos].type == TokenType::POP_METHOD)
            {
                pos++;

                if (tokens[pos].type == TokenType::LEFT_PAREN)
                {
                    pos++;
                    // Check if there is an element inside the parentheses 
                    if (tokens[pos].type != TokenType::RIGHT_PAREN && tokens[pos].type != TokenType::NULL_VALUE)
                    {
                        // Tokenize the element inside the parentheses
                        std::string element = tokens[pos].value;
                        pos++;

                        if (tokens[pos].type == TokenType::RIGHT_PAREN)
                        {
                            pos++;

                            if (tokens[pos].type == TokenType::SEMICOLON)
                            {
                                pos++;

                                // Find the array and perform the pop operation with the element
                                Variable array_var = findVariable(var_name);

                                // Parse the array elements
                                vector<string> elements = parseArrayElements(array_var.value);

                                if (elements.empty())
                                {
                                    Error("Array is empty");
                                }

                                // Find and remove the first instance of the element
                                auto it = std::find(elements.begin(), elements.end(), element);
                                if (it != elements.end())
                                {
                                    elements.erase(it);
                                }
                                else
                                {
                                    Error("Element not found in array");
                                }

                                // Update the array string
                                ostringstream oss;
                                oss << "[";
                                for (size_t i = 0; i < elements.size(); ++i)
                                {
                                    if (i > 0) oss << ",";
                                    oss << elements[i];
                                }
                                oss << "]";
                                array_var.value = oss.str();

                                cout << "Popped element: " << element << endl;
                                cout << "Updated array value: " << array_var.value << endl;

                                updateVariable(array_var.name, array_var.value, array_var.type);

                                return;
                            }
                        }
                    }
                    else
                    {
                        if(tokens[pos].type == TokenType::NULL_VALUE){
                            pos++;
                            
                            if (tokens[pos].type == TokenType::RIGHT_PAREN)
                            {
                                pos++;

                                if (tokens[pos].type == TokenType::SEMICOLON)
                                {
                                    pos++;

                                    // Find the array and perform the pop operation
                                    Variable array_var = findVariable(var_name);

                                    // Parse the array elements
                                    vector<string> elements = parseArrayElements(array_var.value);

                                    if (elements.empty())
                                    {
                                        Error("Array is empty");
                                    }

                                    // Remove and return the first element
                                    string removed_element = elements.front();
                                    elements.erase(elements.begin());

                                    // Update the array string
                                    ostringstream oss;
                                    oss << "[";
                                    for (size_t i = 0; i < elements.size(); ++i)
                                    {
                                        if (i > 0) oss << ",";
                                        oss << elements[i];
                                    }
                                    oss << "]";
                                    array_var.value = oss.str();

                                    cout << "Popped element: " << removed_element << endl;
                                    cout << "Updated array value: " << array_var.value << endl;

                                    updateVariable(array_var.name, array_var.value, array_var.type);

                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Error("Invalid pop method call");
}




// Function to parse the .pop_back() method.
string parseArrayPopBack(const vector<Token> &tokens, size_t &pos) {
    if (tokens[pos].type == TokenType::IDENTIFIER) {
        const string var_name = tokens[pos].value;
        pos++;

        if (tokens[pos].type == TokenType::DOT) {
            pos++;

            if (tokens[pos].type == TokenType::POP_BACK_METHOD) {
                pos++;

                if (tokens[pos].type == TokenType::LEFT_PAREN) {
                    pos++;

                    if (tokens[pos].type == TokenType::RIGHT_PAREN) {
                        pos++;

                        if (tokens[pos].type == TokenType::SEMICOLON) {
                            pos++;

                            // Find the array and perform the pop_back operation
                            Variable array_var = findVariable(var_name);

                            // Parse the array elements
                            vector<string> elements = parseArrayElements(array_var.value);

                            if (elements.empty()) {
                                Error("Array is empty");
                            }

                            // Remove and return the last element
                            string removed_element = elements.back();
                            elements.pop_back();

                            // Update the array string
                            ostringstream oss;
                            oss << "[";
                            for (size_t i = 0; i < elements.size(); ++i) {
                                if (i > 0) oss << ",";
                                oss << elements[i];
                            }
                            oss << "]";
                            array_var.value = oss.str();

                            cout << "Popped back element: " << removed_element << endl;
                            cout << "Updated array value: " << array_var.value << endl;

                            updateVariable(array_var.name, array_var.value, array_var.type);

                            return removed_element;
                        }
                    }
                }
            }
        }
    }

    Error("Invalid pop_back method call");
}
void parseArraySum(const vector<Token> &tokens, size_t &pos)
{
    if (tokens[pos].type == TokenType::IDENTIFIER)
    {
        string var_name = tokens[pos].value;
        pos++;

        if (tokens[pos].type == TokenType::DOT)
        {
            pos++;

            if (tokens[pos].type == TokenType::SUM_METHOD)
            {
                pos++;

                if (tokens[pos].type == TokenType::LEFT_PAREN)
                {
                    pos++;

                    if (tokens[pos].type == TokenType::RIGHT_PAREN)
                    {
                        pos++;

                        if (tokens[pos].type == TokenType::SEMICOLON)
                        {
                            pos++;

                            // Find the array and perform the sum operation
                            Variable array_var = findVariable(var_name);

                            // Parse the array elements
                            vector<string> elements = parseArrayElements(array_var.value);

                            if (elements.empty())
                            {
                                Error("Array is empty");
                            }

                            // Determine the type of the elements and perform the sum
                            bool all_numbers = true;
                            bool all_strings = true;
                            bool all_bools = true;
                            double sum = 0.0;
                            string concatenated_string = "";
                            int bool_sum = 0;

                            for (const string &element : elements)
                            {
                                try
                                {
                                    stod(element);
                                    sum += stod(element);
                                    all_strings = false;
                                    all_bools = false;
                                }
                                catch (const invalid_argument &)
                                {
                                    if (element == "true" || element == "false")
                                    {
                                        bool_sum += (element == "true");
                                        all_numbers = false;
                                        all_strings = false;
                                    }
                                    else
                                    {
                                        all_numbers = false;
                                        all_bools = false;
                                    }
                                }
                                concatenated_string += element;
                            }

                            if (all_numbers)
                            {
                                cout << "Sum of array elements: " << sum << endl;
                            }
                            else if (all_strings)
                            {
                                cout << "Concatenated string: " << concatenated_string << endl;
                            }
                            else if (all_bools)
                            {
                                cout << "Sum of boolean elements: " << bool_sum << endl;
                            }
                            else
                            {
                                cout << "Mixed types in array, cannot sum" << endl;
                            }

                            return;
                        }
                    }
                }
            }
        }
    }

    Error("Invalid sum method call");
}

void parseArrayPush(const vector<Token> &tokens, size_t &pos)
{
    if (tokens[pos].type == TokenType::IDENTIFIER)
    {
        string var_name = tokens[pos].value;
        pos++;

        if (tokens[pos].type == TokenType::DOT)
        {
            pos++;

            if (tokens[pos].type == TokenType::PUSH_METHOD)
            {
                pos++;

                if (tokens[pos].type == TokenType::LEFT_PAREN)
                {
                    pos++;

                    if (tokens[pos].type == TokenType::STRING ||
                        tokens[pos].type == TokenType::NUMBER ||
                        tokens[pos].type == TokenType::BOOLEAN)
                    {
                        string element = tokens[pos].value;
                        pos++;

                        if (tokens[pos].type == TokenType::RIGHT_PAREN)
                        {
                            pos++;

                            if (tokens[pos].type == TokenType::SEMICOLON)
                            {
                                pos++;

                                // Find the array and perform the push operation
                                Variable array_var = findVariable(var_name);

                                // Parse the array elements
                                vector<string> elements = parseArrayElements(array_var.value);

                                // Check the type of the existing elements
                                bool all_numbers = true;
                                bool all_strings = true;
                                bool all_bools = true;

                                for (const string &elem : elements)
                                {
                                    try
                                    {
                                        stod(elem);
                                        all_strings = false;
                                        all_bools = false;
                                    }
                                    catch (const invalid_argument &)
                                    {
                                        if (elem == "true" || elem == "false")
                                        {
                                            all_numbers = false;
                                            all_strings = false;
                                        }
                                        else
                                        {
                                            all_numbers = false;
                                            all_bools = false;
                                        }
                                    }
                                }

                                // Check the type of the new element
                                bool new_element_is_number = false;
                                bool new_element_is_string = false;
                                bool new_element_is_bool = false;

                                try
                                {
                                    stod(element);
                                    new_element_is_number = true;
                                }
                                catch (const invalid_argument &)
                                {
                                    if (element == "true" || element == "false")
                                    {
                                        new_element_is_bool = true;
                                    }
                                    else
                                    {
                                        new_element_is_string = true;
                                    }
                                }

                                // Validate the types
                                if ((all_numbers && new_element_is_number) ||
                                    (all_strings && new_element_is_string) ||
                                    (all_bools && new_element_is_bool))
                                {
                                    // Insert the new element at the front of the array
                                    elements.insert(elements.begin(), element);

                                    // Update the array string
                                    ostringstream oss;
                                    oss << "[";
                                    for (size_t i = 0; i < elements.size(); ++i)
                                    {
                                        if (i > 0) oss << ",";
                                        oss << elements[i];
                                    }
                                    oss << "]";
                                    array_var.value = oss.str();

                                    cout << "Pushed element: " << element << " to the front of array: " << var_name << endl;
                                    cout << "Updated array value: " << array_var.value << endl;

                                    updateVariable(array_var.name, array_var.value, array_var.type);
                                }
                                else
                                {
                                    Error("Type mismatch: Cannot push element of different type to the array");
                                }

                                return;
                            }
                        }
                    }
                }
            }
        }
    }

    Error("Invalid push method call");
}
void parseArrayPushBack(const vector<Token> &tokens, size_t &pos)
{
    if (tokens[pos].type == TokenType::IDENTIFIER)
    {
        string var_name = tokens[pos].value;
        pos++;

        if (tokens[pos].type == TokenType::DOT)
        {
            pos++;

            if (tokens[pos].type == TokenType::PUSH_BACK_METHOD)
            {
                pos++;

                if (tokens[pos].type == TokenType::LEFT_PAREN)
                {
                    pos++;

                    if (tokens[pos].type == TokenType::STRING ||
                        tokens[pos].type == TokenType::NUMBER ||
                        tokens[pos].type == TokenType::BOOLEAN)
                    {
                        string element = tokens[pos].value;
                        pos++;

                        if (tokens[pos].type == TokenType::RIGHT_PAREN)
                        {
                            pos++;

                            if (tokens[pos].type == TokenType::SEMICOLON)
                            {
                                pos++;

                                // Find the array and perform the push_back operation
                                Variable array_var = findVariable(var_name);

                                // Parse the array elements
                                vector<string> elements = parseArrayElements(array_var.value);

                                // Check the type of the existing elements
                                bool all_numbers = true;
                                bool all_strings = true;
                                bool all_bools = true;

                                for (const string &elem : elements)
                                {
                                    try
                                    {
                                        stod(elem);
                                        all_strings = false;
                                        all_bools = false;
                                    }
                                    catch (const invalid_argument &)
                                    {
                                        if (elem == "true" || elem == "false")
                                        {
                                            all_numbers = false;
                                            all_strings = false;
                                        }
                                        else
                                        {
                                            all_numbers = false;
                                            all_bools = false;
                                        }
                                    }
                                }

                                // Check the type of the new element
                                bool new_element_is_number = false;
                                bool new_element_is_string = false;
                                bool new_element_is_bool = false;

                                try
                                {
                                    stod(element);
                                    new_element_is_number = true;
                                }
                                catch (const invalid_argument &)
                                {
                                    if (element == "true" || element == "false")
                                    {
                                        new_element_is_bool = true;
                                    }
                                    else
                                    {
                                        new_element_is_string = true;
                                    }
                                }

                                // Validate the types
                                if ((all_numbers && new_element_is_number) ||
                                    (all_strings && new_element_is_string) ||
                                    (all_bools && new_element_is_bool))
                                {
                                    // Add the new element to the end of the array
                                    elements.push_back(element);

                                    // Update the array string
                                    ostringstream oss;
                                    oss << "[";
                                    for (size_t i = 0; i < elements.size(); ++i)
                                    {
                                        if (i > 0) oss << ",";
                                        oss << elements[i];
                                    }
                                    oss << "]";
                                    array_var.value = oss.str();

                                    cout << "Pushed back element: " << element << " to array: " << var_name << endl;
                                    cout << "Updated array value: " << array_var.value << endl;

                                    updateVariable(array_var.name, array_var.value, array_var.type);
                                }
                                else
                                {
                                    Error("Type mismatch: Cannot push element of different type to the array");
                                }

                                return;
                            }
                        }
                    }
                }
            }
        }
    }

    Error("Invalid push_back method call");
}

void parseArrayMin(const vector<Token> &tokens, size_t &pos)
{
    if (tokens[pos].type == TokenType::IDENTIFIER)
    {
        string var_name = tokens[pos].value;
        pos++;

        if (tokens[pos].type == TokenType::DOT)
        {
            pos++;

            if (tokens[pos].type == TokenType::MIN_METHOD)
            {
                pos++;

                if (tokens[pos].type == TokenType::LEFT_PAREN)
                {
                    pos++;

                    if (tokens[pos].type == TokenType::RIGHT_PAREN)
                    {
                        pos++;

                        if (tokens[pos].type == TokenType::SEMICOLON)
                        {
                            pos++;

                            // Find the array and perform the min operation
                            Variable array_var = findVariable(var_name);

                            // Parse the array elements
                            vector<string> elements = parseArrayElements(array_var.value);

                            if (elements.empty())
                            {
                                Error("Array is empty");
                            }

                            // Determine the type of the elements and find the minimum
                            bool all_numbers = true;
                            double min_value = std::numeric_limits<double>::max();

                            for (const string &element : elements)
                            {
                                try
                                {
                                    double value = stod(element);
                                    if (value < min_value)
                                    {
                                        min_value = value;
                                    }
                                }
                                catch (const invalid_argument &)
                                {
                                    all_numbers = false;
                                }
                            }

                            if (all_numbers)
                            {
                                cout << "Minimum value in array: " << min_value << endl;
                            }
                            else
                            {
                                Error("Array contains non-numeric elements, cannot find minimum");
                            }

                            return;
                        }
                    }
                }
            }
        }
    }

    Error("Invalid min method call");
}

void parseArrayMax(const vector<Token> &tokens, size_t &pos)
{
    if (tokens[pos].type == TokenType::IDENTIFIER)
    {
        string var_name = tokens[pos].value;
        pos++;

        if (tokens[pos].type == TokenType::DOT)
        {
            pos++;

            if (tokens[pos].type == TokenType::MAX_METHOD)
            {
                pos++;

                if (tokens[pos].type == TokenType::LEFT_PAREN)
                {
                    pos++;

                    if (tokens[pos].type == TokenType::RIGHT_PAREN)
                    {
                        pos++;

                        if (tokens[pos].type == TokenType::SEMICOLON)
                        {
                            pos++;

                            // Find the array and perform the max operation
                            Variable array_var = findVariable(var_name);

                            // Parse the array elements
                            vector<string> elements = parseArrayElements(array_var.value);

                            if (elements.empty())
                            {
                                Error("Array is empty");
                            }

                            // Determine the type of the elements and find the maximum
                            bool all_numbers = true;
                            double max_value = std::numeric_limits<double>::lowest();

                            for (const string &element : elements)
                            {
                                try
                                {
                                    double value = stod(element);
                                    if (value > max_value)
                                    {
                                        max_value = value;
                                    }
                                }
                                catch (const invalid_argument &)
                                {
                                    all_numbers = false;
                                }
                            }

                            if (all_numbers)
                            {
                                cout << "Maximum value in array: " << max_value << endl;
                            }
                            else
                            {
                                Error("Array contains non-numeric elements, cannot find maximum");
                            }

                            return;
                        }
                    }
                }
            }
        }
    }

    Error("Invalid max method call");
}

void parseArrayReverse(const vector<Token> &tokens, size_t &pos)
{
    if (tokens[pos].type == TokenType::IDENTIFIER)
    {
        string var_name = tokens[pos].value;
        pos++;

        if (tokens[pos].type == TokenType::DOT)
        {
            pos++;

            if (tokens[pos].type == TokenType::REVERSE_METHOD)
            {
                pos++;

                if (tokens[pos].type == TokenType::LEFT_PAREN)
                {
                    pos++;

                    if (tokens[pos].type == TokenType::RIGHT_PAREN)
                    {
                        pos++;

                        if (tokens[pos].type == TokenType::SEMICOLON)
                        {
                            pos++;

                            // Find the array and perform the reverse operation
                            Variable array_var = findVariable(var_name);

                            // Parse the array elements
                            vector<string> elements = parseArrayElements(array_var.value);

                            if (elements.empty())
                            {
                                Error("Array is empty");
                            }

                            // Reverse the elements
                            std::reverse(elements.begin(), elements.end());

                            // Update the array string
                            ostringstream oss;
                            oss << "[";
                            for (size_t i = 0; i < elements.size(); ++i)
                            {
                                if (i > 0) oss << ",";
                                oss << elements[i];
                            }
                            oss << "]";
                            array_var.value = oss.str();

                            cout << "Reversed array: " << array_var.value << endl;

                            updateVariable(array_var.name, array_var.value, array_var.type);

                            return;
                        }
                    }
                }
            }
        }
    }

    Error("Invalid reverse method call");
}

void parseArrayDeleteDuplicates(const vector<Token> &tokens, size_t &pos)
{
    if (tokens[pos].type == TokenType::IDENTIFIER)
    {
        string var_name = tokens[pos].value;
        pos++;

        if (tokens[pos].type == TokenType::DOT)
        {
            pos++;

            if (tokens[pos].type == TokenType::DELETE_DUPLICATES_METHOD)
            {
                pos++;

                if (tokens[pos].type == TokenType::LEFT_PAREN)
                {
                    pos++;

                    if (tokens[pos].type == TokenType::RIGHT_PAREN)
                    {
                        pos++;

                        if (tokens[pos].type == TokenType::SEMICOLON)
                        {
                            pos++;

                            // Find the array and perform the delete_duplicates operation
                            Variable array_var = findVariable(var_name);

                            // Parse the array elements
                            vector<string> elements = parseArrayElements(array_var.value);

                            if (elements.empty())
                            {
                                Error("Array is empty");
                            }

                            // Remove duplicates
                            std::sort(elements.begin(), elements.end());
                            elements.erase(std::unique(elements.begin(), elements.end()), elements.end());

                            // Update the array string
                            ostringstream oss;
                            oss << "[";
                            for (size_t i = 0; i < elements.size(); ++i)
                            {
                                if (i > 0) oss << ",";
                                oss << elements[i];
                            }
                            oss << "]";
                            array_var.value = oss.str();

                            cout << "Array after deleting duplicates: " << array_var.value << endl;

                            updateVariable(array_var.name, array_var.value, array_var.type);

                            return;
                        }
                    }
                }
            }
        }
    }

    Error("Invalid delete_duplicates method call");
}

void parseArrayRandomize(const vector<Token> &tokens, size_t &pos)
{
    if (tokens[pos].type == TokenType::IDENTIFIER)
    {
        string var_name = tokens[pos].value;
        pos++;

        if (tokens[pos].type == TokenType::DOT)
        {
            pos++;

            if (tokens[pos].type == TokenType::RANDOMIZE_METHOD)
            {
                pos++;

                if (tokens[pos].type == TokenType::LEFT_PAREN)
                {
                    pos++;

                    if (tokens[pos].type == TokenType::RIGHT_PAREN)
                    {
                        pos++;

                        if (tokens[pos].type == TokenType::SEMICOLON)
                        {
                            pos++;

                            // Find the array and perform the randomize operation
                            Variable array_var = findVariable(var_name);

                            // Parse the array elements
                            vector<string> elements = parseArrayElements(array_var.value);

                            if (elements.empty())
                            {
                                Error("Array is empty");
                            }

                            // Randomly sort the elements
                            std::random_device rd;
                            std::mt19937 g(rd());
                            std::shuffle(elements.begin(), elements.end(), g);

                            // Update the array string
                            ostringstream oss;
                            oss << "[";
                            for (size_t i = 0; i < elements.size(); ++i)
                            {
                                if (i > 0) oss << ",";
                                oss << elements[i];
                            }
                            oss << "]";
                            array_var.value = oss.str();

                            cout << "Randomized array: " << array_var.value << endl;

                            updateVariable(array_var.name, array_var.value, array_var.type);

                            return;
                        }
                    }
                }
            }
        }
    }

    Error("Invalid randomize method call");
}

vector<string> parseArrayElements(const string &array_str)
{
    // Helper function to parse array elements from a string representation
    vector<string> elements;
    istringstream ss(array_str.substr(1, array_str.size() - 2)); // remove "[" and "]"
    string element;
    while (getline(ss, element, ','))
    {
        elements.push_back(element);
    }
    return elements;
}