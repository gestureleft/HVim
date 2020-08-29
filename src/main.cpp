#include <iostream>
#include <cstdlib>
#include <array>

#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

struct EditorConfig
{
    termios orig_termios;
};

struct WindowSize
{
    int width;
    int height;
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

std::string editor_draw_rows(const WindowSize& window_dimensions)
{
    std::string output{};
    for (int y = 0; y < window_dimensions.height; y++)
    {
        output.append("~");
        if (y == window_dimensions.height/3) {
            std::string welcome_no_padding = "Welcome to HVim -- Version ";
            welcome_no_padding.append(VERSION);
            std::string welcome((window_dimensions.width - welcome_no_padding.length()) / 2,' ');
            welcome.append(welcome_no_padding);
            output.append(welcome);
        }
        output.append("\x1b[K"); // Clear the old contents from the line
        if (y < window_dimensions.height - 1) output.append("\r\n");
    }
    return output;
}

void editor_refresh_screen()
{
    std::string output_str{};

    output_str.append("\x1b[?25l"); // Hide the cursor
    output_str.append("\x1b[H");    // Reset Cursor to top left
    output_str.append(editor_draw_rows(get_window_size())); // Draw the content
    output_str.append("\x1b[H");    // Reset Cursor to top left
    output_str.append("\x1b[?25h"); // Show the cursor

    write(STDOUT_FILENO, output_str.c_str(), output_str.length());
}

bool process_key_press()
{
    char c = editor_read_key();

    switch (c)
    {
        case ('q'):
            return false;
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

    auto [row_count, col_count] = get_window_size();

    std::string output_string;

    bool run = true;
    while (run) {
        editor_refresh_screen();
        run = process_key_press();
    }
    disable_raw_mode(editor_config.orig_termios);
    return 0;
}