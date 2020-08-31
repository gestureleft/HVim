#include <iostream>
#include <cstdlib>
#include <array>

#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int log_descr;

struct WindowSize
{
    int width;
    int height;
};

WindowSize get_cursor_position()
{
    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) throw std::ios_base::failure(""); // Request cursor position
    // The reply is in the form: \x1b[n;mR, where n is the row and m is the column
    // The following code gets the cursor position out of this responses.

    std::array<char, 32> in_chars{};

    for (auto& c : in_chars)
    {
        if (read(STDIN_FILENO, &c, 1) != 1) break;
        if (c == 'R') break;
    }

    if (in_chars.at(0) != '\x1b' || in_chars.at(1) != '[') throw std::ios_base::failure("");
    WindowSize cursor_pos{};
    if (std::sscanf(&in_chars.at(2), "%d;%d", &(cursor_pos.height), &(cursor_pos.width)) != 2) throw std::ios_base::failure("");

    return cursor_pos;
}

WindowSize get_window_size()
{
    winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
    { // ioctl doesn't work on some systems. For those systems, move the cursor to the bottom right of screen and
        // get cursor position.
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) throw std::ios_base::failure("");
        return get_cursor_position();
    } else {
        return {ws.ws_col, ws.ws_row};
    }
}

struct EditorConfig
{
    struct CursorPosition
    {
        int x;
        int y;
    };
    CursorPosition cursor{};
    termios orig_termios;

    enum Direction {
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    void move_cursor(Direction direction)
    {
        switch (direction)
        {
            case LEFT:
                if (this->cursor.x != 0)
                    this->cursor.x--;
                break;
            case RIGHT:
                if (this->cursor.x != get_window_size().width - 1)
                    this->cursor.x++;
                break;
            case UP:
                if (this->cursor.y != 0)
                    this->cursor.y--;
                break;
            case DOWN:
                if (this->cursor.y != get_window_size().height - 1)
                    this->cursor.y++;
                break;
        }
    }
};

const std::string VERSION = "0.0.1";

inline void disable_raw_mode(const termios& orig_termios)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode(const termios& orig_termios)
{
    termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) exit(errno);
}

char editor_read_key()
{
   std::size_t num_read;
   char c;
   while ((num_read = read(STDIN_FILENO, &c, 1)) != 1)
   {
       if ((num_read == -1 && errno != EAGAIN)) std::exit(errno);
   }
   return c;
}

std::string editor_draw_rows(const WindowSize& window_dimensions)
{
    std::string output{};
    for (int y = 0; y < window_dimensions.height; y++)
    {
        output.append("~");
        if (y == window_dimensions.height/3) {
            std::string welcome_no_padding = "Welcome to HVim -- Version " + VERSION;
            std::string welcome((window_dimensions.width - welcome_no_padding.length()) / 2,' ');
            welcome.append(welcome_no_padding);
            output.append(welcome);
        }
        output.append("\x1b[K"); // Clear the old contents from the line
        if (y < window_dimensions.height - 1) output.append("\r\n");
    }
    return output;
}

void editor_refresh_screen(EditorConfig& editor_config)
{
    std::string output_str{};

    output_str.append("\x1b[?25l"); // Hide the cursor
    output_str.append("\x1b[H");    // Reset Cursor to top left
    output_str.append(editor_draw_rows(get_window_size())); // Draw the content

    std::string move_cursor{"\x1b["};
    move_cursor += std::to_string(editor_config.cursor.y + 1) + ";" + std::to_string(editor_config.cursor.x + 1) + "H";
    write(log_descr, move_cursor.c_str(), move_cursor.length());
    output_str.append(move_cursor);

    output_str.append("\x1b[?25h"); // Show the cursor

    write(STDOUT_FILENO, output_str.c_str(), output_str.length());
}

bool process_key_press(EditorConfig& editor_config)
{
    char c = editor_read_key();

    switch (c)
    {
        case ('q'):
            return false;
        case ('h'):
            editor_config.move_cursor(EditorConfig::Direction::LEFT);
            break;
        case ('j'):
            editor_config.move_cursor(EditorConfig::Direction::DOWN);
            break;
        case ('k'):
            editor_config.move_cursor(EditorConfig::Direction::UP);
            break;
        case ('l'):
            editor_config.move_cursor(EditorConfig::Direction::RIGHT);
            break;
        default:
            break;
    }
    return true;
}

int main()
{
    EditorConfig editor_config;
    tcgetattr(STDIN_FILENO, &(editor_config.orig_termios));
    enable_raw_mode(editor_config.orig_termios);

    log_descr = open("/Users/harrisonmarshall/dev/HVim/log", O_WRONLY);

    auto [row_count, col_count] = get_window_size();

    std::string output_string;

    bool run = true;
    while (run) {
        editor_refresh_screen(editor_config);
        run = process_key_press(editor_config);
    }
    disable_raw_mode(editor_config.orig_termios);
    return 0;
}