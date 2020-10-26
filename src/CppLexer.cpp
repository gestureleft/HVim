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
        case CppToken::Type::String: {
            return "String";
        }
        case CppToken::Type::Comment: {
            return "Comment";
        }
        case CppToken::Type::Comma: {
            return "Comma";
        }
        case CppToken::Type::Number: {
            return "Number";
        }
        case CppToken::Type::Keyword: {
            return "Keyword";
        }
        case CppToken::Type::Identifier: {
            return "Identifier";
        }
    }
}

const bool is_space(const char c)
{
    return c == ' ' || c == '\n' || c == '\t';
}

const bool is_keyword(const std::string_view& str)
{
    static std::unordered_map<std::string_view, int> keywords;
    if (keywords.empty())
    {
        keywords.emplace(std::make_pair("asm", 0));
        keywords.emplace(std::make_pair("auto", 0));
        keywords.emplace(std::make_pair("bool", 0));
        keywords.emplace(std::make_pair("break", 0));
        keywords.emplace(std::make_pair("case", 0));
        keywords.emplace(std::make_pair("catch", 0));
        keywords.emplace(std::make_pair("char", 0));
        keywords.emplace(std::make_pair("class", 0));
        keywords.emplace(std::make_pair("const", 0));
        keywords.emplace(std::make_pair("const_cast", 0));
        keywords.emplace(std::make_pair("continue", 0));
        keywords.emplace(std::make_pair("default", 0));
        keywords.emplace(std::make_pair("delete", 0));
        keywords.emplace(std::make_pair("do", 0));
        keywords.emplace(std::make_pair("double", 0));
        keywords.emplace(std::make_pair("dynamic_cast", 0));
        keywords.emplace(std::make_pair("if", 0));
        keywords.emplace(std::make_pair("else", 0));
        keywords.emplace(std::make_pair("enum", 0));
        keywords.emplace(std::make_pair("explicit", 0));
        keywords.emplace(std::make_pair("export", 0));
        keywords.emplace(std::make_pair("extern", 0));
        keywords.emplace(std::make_pair("false", 0));
        keywords.emplace(std::make_pair("float", 0));
        keywords.emplace(std::make_pair("for", 0));
        keywords.emplace(std::make_pair("friend", 0));
        keywords.emplace(std::make_pair("inline", 0));
        keywords.emplace(std::make_pair("int", 0));
        keywords.emplace(std::make_pair("long", 0));
        keywords.emplace(std::make_pair("motable", 0));
        keywords.emplace(std::make_pair("new", 0));
        keywords.emplace(std::make_pair("operator", 0));
        keywords.emplace(std::make_pair("protected", 0));
        keywords.emplace(std::make_pair("public", 0));
        keywords.emplace(std::make_pair("register", 0));
        keywords.emplace(std::make_pair("reinterpret_cast", 0));
        keywords.emplace(std::make_pair("return", 0));
        keywords.emplace(std::make_pair("short", 0));
        keywords.emplace(std::make_pair("goto", 0));
        keywords.emplace(std::make_pair("if", 0));
        keywords.emplace(std::make_pair("static", 0));
        keywords.emplace(std::make_pair("static_cast", 0));
        keywords.emplace(std::make_pair("struct", 0));
        keywords.emplace(std::make_pair("switch", 0));
        keywords.emplace(std::make_pair("namespace", 0));
        keywords.emplace(std::make_pair("this", 0));
        keywords.emplace(std::make_pair("private", 0));
        keywords.emplace(std::make_pair("typedef", 0));
        keywords.emplace(std::make_pair("union", 0));
        keywords.emplace(std::make_pair("unsigned", 0));
        keywords.emplace(std::make_pair("signed", 0));
        keywords.emplace(std::make_pair("sizeof", 0));
        keywords.emplace(std::make_pair("void", 0));
        keywords.emplace(std::make_pair("wchar_t", 0));
        keywords.emplace(std::make_pair("while", 0));
        keywords.emplace(std::make_pair("throw", 0));
        keywords.emplace(std::make_pair("true", 0));
        keywords.emplace(std::make_pair("try", 0));
        keywords.emplace(std::make_pair("typeid", 0));
        keywords.emplace(std::make_pair("typename", 0));
        keywords.emplace(std::make_pair("using", 0));
        keywords.emplace(std::make_pair("virtual", 0));
        keywords.emplace(std::make_pair("volatile", 0));
        keywords.emplace(std::make_pair("template", 0));
    }
    return keywords.contains(str);
}

bool valid_first_character_of_identifier(char ch)
{
    return isalpha(ch) || ch == '_' || ch == '$';
}

bool valid_nonfirst_character_of_identifier(char ch)
{
    return valid_first_character_of_identifier(ch) || isdigit(ch);
}

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

/*
const CppToken::Type get_type(const std::vector<CppToken>& tokens, const int row, const int col)
{
    for (auto token: tokens)
    {
        if (token.m_start.col <= col && token.m_end.col >= col &&
            token.m_start.row <= row && token.m_end.row >= row)
            return token.m_type;
    }
    return CppToken::Type::Unknown;
}
*/


int main()
{
    std::ifstream file("test_lex.cpp");
    std::string str{};

    file.seekg(0, std::ios::end);   
    str.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());

    file.close();

    const auto tokens = lex(str);
    for (auto token: tokens)
    {
        if (token.m_type != CppToken::Type::Whitespace)
            std::cout << token.to_string() << ": " << token.m_view << '\n';
    }
}
