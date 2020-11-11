#include "CppLexer.h"

#include <ctype.h>
#include <string_view>
#include <vector>
#include <utility>
#include <numeric>
#include <array>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string.h>

const size_t CppToken::length() const {
    return m_view.length();
}

const char* CppToken::to_string() const {
    switch (m_type)
    {
        case CppToken::Type::Unknown: {
            return "Unknown";
        }
        case CppToken::Type::Whitespace: {
            return "Whitespace";
        }
        case CppToken::Type::PreprocessorDirective: {
            return "PreprocessorDirective";
        }
        case CppToken::Type::Char: {
            return "Char";
        }
        case CppToken::Type::LeftBrace: {
            return "LeftBrace";
        }
        case CppToken::Type::RightBrace: {
            return "RightBrace";
        }
        case CppToken::Type::LeftBracket: {
            return "LeftBracket";
        }
        case CppToken::Type::RightBracket: {
            return "RightBracket";
        }
        case CppToken::Type::LeftParen: {
            return "LeftParent";
        }
        case CppToken::Type::RightParen: {
            return "RightParen";
        }
        case CppToken::Type::Asterisk: {
            return "Asterisk";
        }
        case CppToken::Type::SemiColon: {
            return "SemiColon";
        }
        case CppToken::Type::Colon: {
            return "Colon";
        }
        case CppToken::Type::String: {
            return "String";
        }
        case CppToken::Type::Comment: {
            return "Comment";
        }
        case CppToken::Type::Comma: {
            return "Comma";
        }
        case CppToken::Type::LessThan: {
            return "LessThan";
        }
        case CppToken::Type::GreaterThan: {
            return "GreaterThan";
        }
        case CppToken::Type::Number: {
            return "Number";
        }
        case CppToken::Type::Percent: {
            return "Percent";
        }
        case CppToken::Type::Tilde: {
            return "Tilde";
        }
        case CppToken::Type::Keyword: {
            return "Keyword";
        }
        case CppToken::Type::Identifier: {
            return "Identifier";
        }
        case CppToken::Type::Ampersand: {
            return "Ampersand";
        }
        case CppToken::Type::Pipe: {
            return "Pipe";
        }
        case CppToken::Type::ExplanationMark: {
            return "ExplanationMark";
        }
        case CppToken::Type::Minus: {
            return "Minus";
        }
        case CppToken::Type::Plus: {
            return "Plus";
        }
        case CppToken::Type::Equals: {
            return "Equals";
        }
        case CppToken::Type::Dot: {
            return "Dot";
        }
    }
}

const bool is_space(const char c)
{
    return c == ' ' || c == '\n' || c == '\t';
}

const bool is_keyword(const std::string_view& str)
{
    return str == "asm" ||
           str == "auto" ||
           str == "bool" ||
           str == "break" ||
           str == "case" ||
           str == "catch" ||
           str == "char" ||
           str == "class" ||
           str == "const" ||
           str == "const_cast" ||
           str == "continue" ||
           str == "default" ||
           str == "delete" ||
           str == "do" ||
           str == "double" ||
           str == "dynamic_cast" ||
           str == "if" ||
           str == "else" ||
           str == "enum" ||
           str == "explicit" ||
           str == "export" ||
           str == "extern" ||
           str == "false" ||
           str == "float" ||
           str == "for" ||
           str == "friend" ||
           str == "inline" ||
           str == "int" ||
           str == "long" ||
           str == "motable" ||
           str == "new" ||
           str == "operator" ||
           str == "protected" ||
           str == "public" ||
           str == "register" ||
           str == "reinterpret_cast" ||
           str == "return" ||
           str == "short" ||
           str == "goto" ||
           str == "if" ||
           str == "static" ||
           str == "static_cast" ||
           str == "struct" ||
           str == "switch" ||
           str == "namespace" ||
           str == "this" ||
           str == "private" ||
           str == "typedef" ||
           str == "union" ||
           str == "unsigned" ||
           str == "signed" ||
           str == "sizeof" ||
           str == "void" ||
           str == "wchar_t" ||
           str == "while" ||
           str == "throw" ||
           str == "true" ||
           str == "try" ||
           str == "typeid" ||
           str == "typename" ||
           str == "using" ||
           str == "virtual" ||
           str == "volatile" ||
           str == "template";
}

/*
std::vector<CppToken> lex(const std::string& content)
{
    int index { 0 };
    int current_token_start_index { 0 };

    std::vector<CppToken> tokens{};

    const auto peek = [&](int offset = 0) -> char
    {
        if ((index + offset >= content.length()))
            return 0;
        return content[offset+index];
    };

    const auto commit_token = [&](CppToken::Type type){
        tokens.push_back({type, std::string_view(content.c_str() + current_token_start_index, index - current_token_start_index)});
    };

    const auto match_char = [&] (CppToken::Type type, const char c) {
        if (peek() == c)
        {
            current_token_start_index = index;
            index++;
            commit_token(type);
            return true;
        }
        return false;
    };

    while (index < content.length())
    {
        std::cout << index << ": " << content[index] << '\n';
        if (match_char(CppToken::Type::RightParen,   ')')) continue;
        if (match_char(CppToken::Type::LeftParen,    '(')) continue;
        if (match_char(CppToken::Type::RightBrace,   '}')) continue;
        if (match_char(CppToken::Type::LeftBrace,    '{')) continue;
        if (match_char(CppToken::Type::RightBracket, ']')) continue;
        if (match_char(CppToken::Type::LeftBracket,  '[')) continue;
        if (match_char(CppToken::Type::SemiColon,    ';')) continue;
        if (match_char(CppToken::Type::Colon,        ':')) continue;
        if (match_char(CppToken::Type::LessThan,     '<')) continue;
        if (match_char(CppToken::Type::GreaterThan,  '>')) continue;
        if (peek() && peek() == '\'')
        {
            current_token_start_index = index;
            index++;
            if (peek() == '\\')
                index++;
            index += 2;
            commit_token(CppToken::Type::Char);
            continue;
        }
        if (peek() && peek() == '"')
        {
            current_token_start_index = index;
            index++;
            while (peek() && peek() != '"')
                index++;
            index++;
            commit_token(CppToken::Type::String);
            continue;
        }
        if (isdigit(peek()))
        {
            current_token_start_index = index;
            while (isdigit(peek()))
                index++;
            commit_token(CppToken::Type::Number);
            continue;
        }
        if (valid_first_character_of_identifier(peek()))
        {
            current_token_start_index = index;
            while (valid_nonfirst_character_of_identifier(peek()))
                index++;
            std::string_view identifier = std::string_view(content.c_str() + current_token_start_index, index - current_token_start_index);
            if (is_keyword(identifier))
                tokens.push_back({CppToken::Type::Keyword, identifier});
            else
                tokens.push_back({CppToken::Type::Identifier, identifier});
            continue;
        }
        if (peek() && is_space(peek()))
        {
            current_token_start_index = index;
            while (is_space(peek()))
                index++;
            commit_token(CppToken::Type::Whitespace);
            continue;
        }
        if (peek() && peek() == '#')
        {
            current_token_start_index = index;
            while (peek() != '\n')
                index++;
            commit_token(CppToken::Type::PreprocessorDirective);
            continue;
        }
        index++;
    }

    return tokens;
}
*/

CppLexer::CppLexer(const char* content) : m_content{content} {
    m_index = 0;
    m_length = strlen(content);
}

void CppLexer::reset() {
    m_index = 0;
}

void CppLexer::reset(const char* new_content) {
    m_index = 0;
    m_content = new_content;
    m_length = strlen(new_content);
}

bool is_valid_first_character_of_identifier(char ch) {
    return isalpha(ch) || ch == '_' || ch == '$';
}

bool is_valid_nonfirst_character_of_identifier(char ch) {
    return is_valid_first_character_of_identifier(ch) || isdigit(ch);
}

char CppLexer::peek(const size_t offset = 0) {
    if (m_index + offset >= m_length)
        return '\0';
    return m_content[m_index + offset];
}

CppToken::Type match_single_char(const char c) {
    static const std::unordered_map<char, CppToken::Type> single_chars = {
        {'{', CppToken::Type::LeftBrace},
        {'}', CppToken::Type::RightBrace},
        {'[', CppToken::Type::LeftBracket},
        {']', CppToken::Type::RightBracket},
        {'(', CppToken::Type::LeftParen},
        {')', CppToken::Type::RightParen},
        {':', CppToken::Type::SemiColon},
        {'*', CppToken::Type::Asterisk},
        {'<', CppToken::Type::LessThan},
        {'>', CppToken::Type::GreaterThan},
        {'%', CppToken::Type::Percent},
        {'^', CppToken::Type::Tilde},
        {'&', CppToken::Type::Ampersand},
        {'|', CppToken::Type::Pipe},
        {'!', CppToken::Type::ExplanationMark},
        {'-', CppToken::Type::Minus},
        {'+', CppToken::Type::Plus},
        {'=', CppToken::Type::Equals},
        {';', CppToken::Type::SemiColon},
        {',', CppToken::Type::Comma},
        {'.', CppToken::Type::Dot}
    };
    if (single_chars.contains(c))
        return single_chars.at(c);
    return CppToken::Type::Unknown;
}

CppToken CppLexer::next_token()
{
    size_t start_index = m_index;
    
    if (peek() == '#') {
        while (peek() && peek() != '\n') {
            m_index++;
        }
        return { CppToken::Type::PreprocessorDirective, std::string_view(m_content + start_index, m_index - start_index)};
    } else if (is_space(peek())) {
        while (peek() && is_space(peek())) {
            m_index++;
        }
        return { CppToken::Type::Whitespace, std::string_view(m_content + start_index, m_index - start_index)};
    } else if (is_valid_first_character_of_identifier(peek())) {
        while (peek() && is_valid_nonfirst_character_of_identifier(peek())) {
            m_index++;
        }
        auto token = std::string_view(m_content + start_index, m_index - start_index);
        if (is_keyword(token))
            return { CppToken::Type::Keyword, token };
        return { CppToken::Type::Identifier, token };
    } else if (peek() == '\'') {
        m_index++;
        if (peek() == '\\')
            m_index++;
        m_index += 2;
        return { CppToken::Type::Char, std::string_view(m_content + start_index, m_index - start_index) };
    } else if (peek() == '"') {
        m_index++;
        while (peek() && peek() != '"') {
            m_index++;
            if (peek() == '\\') {
                m_index += 2;
            }
        }
        m_index++;
        return { CppToken::Type::String, std::string_view(m_content + start_index, m_index - start_index) };
    }
    CppToken::Type type = match_single_char(peek());
    m_index++;
    return { type, std::string_view(m_content + start_index, 1) };

}
