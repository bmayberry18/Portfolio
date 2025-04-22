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
#include "math.h"

using namespace std;

// Modify parseExpression to return a float result
string parseExpression(const std::vector<Token>& tokens, size_t& pos) {
    string result;
    bool isStringOperation = false;

    // Helper function to parse a single term (number, identifier, or string)
    auto parseTerm = [&](const std::vector<Token>& tokens, size_t& pos) -> string {
        if (tokens[pos].type == TokenType::NUMBER) {
            string value = tokens[pos].value;
            pos++;
            return value;
        } else if (tokens[pos].type == TokenType::IDENTIFIER) {
            Variable variable = findVariable(tokens[pos].value);
            if (variable.type == "int" || variable.type == "float") {
                string value = variable.value;
                pos++;
                return value;
            } else if (variable.type == "string") {
                isStringOperation = true;
                string value = variable.value;
                pos++;
                return value;
            } else {
                Error("Invalid type for operation: " + variable.type);
                throw std::runtime_error("Invalid type for operation: " + variable.type);
            }
        } else if (tokens[pos].type == TokenType::STRING) {
            isStringOperation = true;
            string value = tokens[pos].value;
            pos++;
            return value;
        } else {
            Error("Expected number, identifier, or string");
            throw std::runtime_error("Expected number, identifier, or string");
        }
    };

    // Parse the first term
    result = parseTerm(tokens, pos);

    // Parse subsequent operations
    while (tokens[pos].type == TokenType::ADDITION || tokens[pos].type == TokenType::SUBTRACTION ||
           tokens[pos].type == TokenType::MULTIPLICATION || tokens[pos].type == TokenType::DIVISION ||
           tokens[pos].type == TokenType::POWER || tokens[pos].type == TokenType::MODULO) {
        TokenType operation = tokens[pos].type;
        pos++;
        string rhs = parseTerm(tokens, pos);

        if (isStringOperation) {
            if (operation == TokenType::ADDITION) {
                result += rhs;
            } else if (operation == TokenType::SUBTRACTION) {
                size_t pos = result.find(rhs);
                if (pos != string::npos) {
                    result.erase(pos, rhs.length());
                }
            } else if (operation == TokenType::MULTIPLICATION) {
                if (isStringOperation && tokens[pos - 1].type == TokenType::NUMBER) {
                    int repeatCount = std::stoi(rhs);
                    string repeatedString;
                    for (int i = 0; i < repeatCount; ++i) {
                        repeatedString += result;
                    }
                    result = repeatedString;
                } else {
                    Error("String multiplication is only supported with a number");
                    throw std::runtime_error("String multiplication is only supported with a number");
                }
            } else if (operation == TokenType::DIVISION || operation == TokenType::POWER || operation == TokenType::MODULO) {
                Error("String operation not supported for division, power, or modulo");
                throw std::runtime_error("String operation not supported for division, power, or modulo");
            }
        } else {
            float lhsValue = std::stof(result);
            float rhsValue = std::stof(rhs);
            if (operation == TokenType::ADDITION) {
                lhsValue += rhsValue;
            } else if (operation == TokenType::SUBTRACTION) {
                lhsValue -= rhsValue;
            } else if (operation == TokenType::MULTIPLICATION) {
                lhsValue *= rhsValue;
            } else if (operation == TokenType::DIVISION) {
                if (rhsValue != 0) {
                    lhsValue /= rhsValue;
                } else {
                    Error("Division by zero");
                    throw std::runtime_error("Division by zero");
                }
            } else if (operation == TokenType::POWER) {
                lhsValue = std::pow(lhsValue, rhsValue);
            } else if (operation == TokenType::MODULO) {
                if (static_cast<int>(rhsValue) != 0) {
                    lhsValue = static_cast<int>(lhsValue) % static_cast<int>(rhsValue);
                } else {
                    Error("Modulo by zero");
                    throw std::runtime_error("Modulo by zero");
                }
            }
            result = std::to_string(lhsValue);
        }
    }

    // Check for semicolon
    if (tokens[pos].type == TokenType::SEMICOLON) {
        pos++;
    } else {
        Error("Expected semicolon");
        throw std::runtime_error("Expected semicolon");
    }

    if (isStringOperation) {
        std::cout << "\"" << result << "\"" << std::endl;
    } else {
        std::cout << result << std::endl;
    }

    return result;
}

void parseAssignmentOperation(const std::vector<Token>& tokens, size_t& pos) {
    if (tokens[pos].type == TokenType::IDENTIFIER) {
        string var_name = tokens[pos].value;
        pos++;

        TokenType assignmentOp = tokens[pos].type;
        if (assignmentOp == TokenType::ADDITION_ASSIGNMENT || assignmentOp == TokenType::SUBTRACTION_ASSIGNMENT ||
            assignmentOp == TokenType::MULTIPLICATION_ASSIGNMENT || assignmentOp == TokenType::DIVISION_ASSIGNMENT) {
            pos++;

            // Parse the right-hand side expression
            string rhs = parseExpression(tokens, pos);
            float rhsValue = std::stof(rhs);

            pos--;
            if (tokens[pos].type == TokenType::SEMICOLON) {
                pos++;

                // Find the variable and update its value
                Variable variable = findVariable(var_name);
                if (variable.type == "int" || variable.type == "float") {
                    float currentValue = std::stof(variable.value);
                    float newValue;
                    if (assignmentOp == TokenType::ADDITION_ASSIGNMENT) {
                        newValue = currentValue + rhsValue;
                    } else if (assignmentOp == TokenType::SUBTRACTION_ASSIGNMENT) {
                        newValue = currentValue - rhsValue;
                    } else if (assignmentOp == TokenType::MULTIPLICATION_ASSIGNMENT) {
                        newValue = currentValue * rhsValue;
                    } else if (assignmentOp == TokenType::DIVISION_ASSIGNMENT) {
                        if (rhsValue != 0) {
                            newValue = currentValue / rhsValue;
                        } else {
                            Error("Division by zero");
                            throw std::runtime_error("Division by zero");
                        }
                    }
                    updateVariable(var_name, std::to_string(newValue), variable.type);
                    return;
                } else {
                    Error("Invalid type for assignment operation: " + variable.type);
                    throw std::runtime_error("Invalid type for assignment operation: " + variable.type);
                }
            }
        }
    }
    cout<<token_to_string(tokens[pos].type)<<tokens[pos].value<<endl;
    Error("Invalid assignment operation");
    throw std::runtime_error("Invalid assignment operation");
}