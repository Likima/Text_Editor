#include "lexer.hpp"

std::unordered_map<TokenType, glm::vec3> tokenColors = {
    {KEYWORD, glm::vec3(0.0f, 1.0f, 0.0f)},     // Green
    {IDENTIFIER, glm::vec3(1.0f, 1.0f, 1.0f)},  // White
    {NUMBER, glm::vec3(1.0f, 1.0f, 0.0f)},      // Yellow
    {STRING, glm::vec3(1.0f, 0.0f, 1.0f)},      // Magenta
    {OPERATOR, glm::vec3(0.0f, 0.0f, 1.0f)},    // Blue
    {DELIMITER, glm::vec3(0.0f, 0.0f, 1.0f)},   // Blue
    {COMMENT, glm::vec3(0.5f, 0.5f, 0.5f)}      // Gray
};

std::vector<std::pair<TokenType, std::regex>> tokenDefinitions = {
    {KEYWORD, std::regex("\\b(if|else|for|while|return)\\b")},
    {IDENTIFIER, std::regex("\\b[a-zA-Z_][a-zA-Z0-9_]*\\b")},
    {NUMBER, std::regex("\\b[0-9]+\\b")},
    {STRING, std::regex("\"[^\"]*\"")},
    {OPERATOR, std::regex("[+\\-*/=]")},
    {DELIMITER, std::regex("[(){}; ]")},
    {COMMENT, std::regex("//[^\n]*")}
};

std::vector<std::string> splitBySpaces(const std::string& code) {
    std::vector<std::string> strings;
    std::string currentString;
    bool isSpace = std::isspace(code[0]);

    for (char c : code) {
        if (std::isspace(c)) {
            if (!isSpace) {
                strings.push_back(currentString);
                currentString.clear();
            }
            isSpace = true;
        } else {
            if (isSpace) {
                strings.push_back(currentString);
                currentString.clear();
            }
            isSpace = false;
        }
        currentString += c;
    }

    if (!currentString.empty()) {
        strings.push_back(currentString);
    }

    return strings;
}

Token tokenizeSingleWord(const std::string &word) {

    for (const auto &definition : tokenDefinitions) {
        if (std::regex_match(word, definition.second)) {
            return {definition.first, word};
        }
    }

    // Default case if no match found
    return {IDENTIFIER, word}; // Assuming it's an identifier if not matched
}

std::vector<Token> tokenize(const std::string &code) {
    std::vector<Token> tokens;
    std::sregex_iterator end;
    std::vector<std::string> split_vec = splitBySpaces(code);

    for (const auto &word : split_vec) {
        Token t = tokenizeSingleWord(word);
        tokens.push_back(t);
    }
    return tokens;
}