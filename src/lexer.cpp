#include "lexer.hpp"

std::unordered_map<TokenType, glm::vec3> tokenColors = {
    {KEYWORD, glm::vec3(0.0f, 1.0f, 0.0f)},    // Green
    {IDENTIFIER, glm::vec3(1.0f, 1.0f, 1.0f)}, // White
    {NUMBER, glm::vec3(1.0f, 1.0f, 0.0f)},     // Yellow
    {STRING, glm::vec3(1.0f, 0.0f, 1.0f)},     // Magenta
    {OPERATOR, glm::vec3(0.0f, 0.0f, 1.0f)},   // Blue
    {DELIMITER, glm::vec3(0.0f, 0.0f, 1.0f)},  // Blue
    {COMMENT, glm::vec3(0.5f, 0.5f, 0.5f)}     // Gray
};

std::vector<std::pair<TokenType, std::regex>> tokenDefinitions = {
    {KEYWORD, std::regex("\\b(if|else|for|while|return|auto|break|case|char|const|continue|default|do|double|enum|extern|float|goto|int|long|register|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|alignas|alignof|and|and_eq|asm|atomic_cancel|atomic_commit|atomic_noexcept|bitand|bitor|bool|catch|char16_t|char32_t|char8_t|class|co_await|co_return|co_yield|compl|concept|const_cast|consteval|constexpr|constinit|decltype|delete|dynamic_cast|explicit|export|false|friend|inline|mutable|namespace|new|noexcept|not|not_eq|nullptr|operator|or|or_eq|private|protected|public|reflexpr|reinterpret_cast|requires|static_assert|static_cast|synchronized|template|this|thread_local|throw|true|try|typeid|typename|using|virtual|wchar_t|xor|xor_eq)\\b")},
    {IDENTIFIER, std::regex("\\b[a-zA-Z_][a-zA-Z0-9_]*\\b")},
    {NUMBER, std::regex("\\b[0-9]+|#include\\b")},
    {STRING, std::regex("\"[^\"]*\"")},
    {OPERATOR, std::regex("[+\\-*/=]")},
    {DELIMITER, std::regex("[(){}; ]")},
    {COMMENT, std::regex("//[^\n]*$")}};

#include <vector>
#include <string>
#include <cctype>

std::vector<std::string> splitBySpacesAndBrackets(const std::string &code)
{
    std::vector<std::string> strings;
    std::string currentString;
    bool isSpace = false;
    bool isInComment = false;
    bool isInString = false;
    bool isInChar = false;

    for (size_t i = 0; i < code.length(); ++i)
    {
        char c = code[i];

        if (isInComment)
        {
            currentString += c;
            if (c == '\n')
            {
                strings.push_back(currentString);
                currentString.clear();
                isInComment = false;
            }
            continue;
        }

        if (isInString)
        {
            currentString += c;
            if (c == '\"')
            {
                strings.push_back(currentString);
                currentString.clear();
                isInString = false;
            }
            continue;
        }

        if (isInChar)
        {
            currentString += c;
            if (c == '\'')
            {
                strings.push_back(currentString);
                currentString.clear();
                isInChar = false;
            }
            continue;
        }

        if (std::isspace(c))
        {
            if (!currentString.empty())
            {
                strings.push_back(currentString);
                currentString.clear();
            }
            strings.push_back(" ");
            isSpace = true;
            continue;
        }

        if (c == '(' || c == ')' || c == '{' || c == '}')
        {
            if (!currentString.empty())
            {
                strings.push_back(currentString);
                currentString.clear();
            }
            strings.push_back(std::string(1, c));
            isSpace = false;
            continue;
        }

        if (c == '\"')
        {
            if (!currentString.empty())
            {
                strings.push_back(currentString);
                currentString.clear();
            }
            currentString += c;
            isInString = true;
            continue;
        }

        if (c == '\'')
        {
            if (!currentString.empty())
            {
                strings.push_back(currentString);
                currentString.clear();
            }
            currentString += c;
            isInChar = true;
            continue;
        }

        if (c == '/' && i + 1 < code.length() && code[i + 1] == '/')
        {
            if (!currentString.empty())
            {
                strings.push_back(currentString);
                currentString.clear();
            }
            currentString += c;
            isInComment = true;
            continue;
        }

        currentString += c;
        isSpace = false;
    }

    if (!currentString.empty())
    {
        strings.push_back(currentString);
    }

    return strings;
}

Token tokenizeSingleWord(const std::string &word)
{
    if (word.empty())
    {
        return {DELIMITER, " "}; // Tokenize spaces as delimiters
    }

    for (const auto &definition : tokenDefinitions)
    {
        if (std::regex_match(word, definition.second))
        {
            return {definition.first, word};
        }
    }
    // Default case if no match found
    return {IDENTIFIER, word}; // Assuming it's an identifier if not matched
}

std::vector<Token> tokenize(const std::string &code)
{
    std::vector<Token> tokens;
    std::sregex_iterator end;
    std::vector<std::string> split_vec = splitBySpacesAndBrackets(code);

    for (const auto &word : split_vec)
    {
        Token t = tokenizeSingleWord(word);
        tokens.push_back(t);
    }
    return tokens;
}