#include <termios.h>
#include <utility>
#include <string>

class Term {
public:
    enum class Colour {
        Black = 0,
        White = 15,
        Red = 160,
        Blue = 20,
        DarkGrey = 236
    };
    Term();
    ~Term();
    void draw(size_t x, size_t y, const char* content);
    void draw(size_t x, size_t y, const char* content, Term::Colour colour);
    void refresh();
    size_t getWidth();
    size_t getHeight();

private:
    termios original_terminal_settings;
    termios terminal;
    std::string buffer;
    std::pair<int,int> get_cursor_position();
    const Colour default_colour = Colour::White;
};
