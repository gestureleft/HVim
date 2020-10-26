#include <string_view>
#include <vector>

struct CppToken
{
    enum class Type {
        Unknown,
        Whitespace,
        PreprocessorDirective,
        Char,
        LeftBrace,
        RightBrace,
        LeftBracket,
        RightBracket,
        LeftParen,
        RightParen,
        Asterisk,
        SemiColon,
        String,
        Comma,
        Comment,
        Number,
        Keyword,
        Identifier
    };

    const char* to_string() const;

    Type m_type;
    std::string_view m_view;
};

const CppToken::Type get_type(const std::vector<CppToken>& tokens, const int row, const int col);
