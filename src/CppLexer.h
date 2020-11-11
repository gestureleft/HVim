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
        Colon,
        String,
        Comma,
        LessThan,
        GreaterThan,
        Comment,
        Number,
        Keyword,
        Identifier
    };

    const char* to_string() const;
    const size_t length() const ;

    Type m_type;
    std::string_view m_view;
};

struct CppLexer
{
    const char* m_content;
    
    void reset(const char* new_content);
    void reset();
    CppToken next_token();
    CppLexer(const char* content);
private:
    size_t m_index{0};
    size_t m_length{0};

    char peek(const size_t offset);
};
