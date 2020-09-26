#include <iostream>
#include <cstdlib>
#include <array>
#include <vector>
#include <fstream>
#include <map>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <optional>

#include "EditorConfig.h"
#include "util.h"

namespace Constants
{
    const std::string VERSION = "0.0.1";
    const std::array<char, 6> nav_keys = {'h', 'j', 'k', 'l', ' ', 127};
    const std::map<char, Direction> nav_key_bindings = {
            {nav_keys.at(0), LEFT},
            {nav_keys.at(1), DOWN},
            {nav_keys.at(2), UP},
            {nav_keys.at(3), RIGHT},
            {nav_keys.at(4), FORWARD},
            {nav_keys.at(5), BACK}
    };
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

std::string editor_draw_rows(const EditorConfig& editor_config, const WindowSize& window_dimensions)
{
    std::string output{};
    for (int y = editor_config.m_view_offset_y; y < window_dimensions.height; y++)
    {
        if (y < editor_config.m_content.size())
        {
            output.append(" \x1b[33m" +
                          std::string(num_digits(editor_config.m_content.size()) - num_digits(y + 1) - 1, ' ') +
                          std::to_string(y + 1) +
                          " \x1b[0;11m" +
                          editor_config.m_content.at(y + editor_config.m_view_offset_y));
        } else {
            output.append("~");
            if (editor_config.m_content.empty() && y == window_dimensions.height/3) {
                std::string welcome_no_padding = "Welcome to HVim -- Version " + Constants::VERSION;
                std::string welcome((window_dimensions.width - welcome_no_padding.length()) / 2,' ');
                welcome.append(welcome_no_padding);
                output.append(welcome);
            }

        }
        output.append("\x1b[K"); // Clear the old contents from the line
        if (y < window_dimensions.height - 1) output.append("\r\n");
    }
    return output;
}

void editor_refresh_screen(const EditorConfig& editor_config)
{
    std::string output_str{};

    output_str.append("\x1b[?25l"); // Hide the cursor
    output_str.append("\x1b[H");    // Reset Cursor to top left
    output_str.append(editor_draw_rows(editor_config, get_window_size())); // Draw the content

    std::string move_cursor{"\x1b["};
    move_cursor += std::to_string(editor_config.m_cursor.y + 1) + ";" + std::to_string(editor_config.m_cursor.x + 2 +
            num_digits(editor_config.m_content.size())) + "H";
    output_str.append(move_cursor);

    output_str.append("\x1b[?25h"); // Show the cursor

    write(STDOUT_FILENO, output_str.c_str(), output_str.length());
}

template<typename T, size_t v>
bool contains(const std::array<T,v> arr, T val)
{
    for (T el: arr)
    {
        if (el == val) return true;
    }
    return false;
}

Direction get_move_direction(const char c)
{
    return Constants::nav_key_bindings.at(c);
}

/**
 * Given a cursor and an editor config, checks if the cursor is a valid position for the editor, if so, return a new
 * editor config with the cursor position applied, else, return the original editor config
 * @param e the editor config
 * @param curs the cursor
 */
EditorConfig validate_cursor(const EditorConfig& e, const CursorPosition& curs)
{
    if (curs.x > -1 &&
        curs.x < e.m_content.at(curs.y).size() &&
        curs.y > -1 &&
        curs.y < e.m_content.size())
        return {curs, e.m_view_offset_x, e.m_view_offset_y, e.m_content, e.m_do_run};
    return e;
}

EditorConfig process_key_press(const EditorConfig& e, const char c)
{
    if (contains(Constants::nav_keys, c))
        return {move_cursor(e, get_move_direction(c)), e.m_view_offset_x, e.m_view_offset_y, e.m_content, e.m_do_run};
    if (c == 'q') return {e.m_cursor, e.m_view_offset_x, e.m_view_offset_y, e.m_content, false};
    return e;
}

/**
 * Given a path to a file, returns a vector of strings, where each string is a line of the file.
 * @param file_path the path to the file
 * @return a vector of strings, where each string is a line in the file
 */
std::vector<std::string> open_file(const char* file_path)
{
    std::vector<std::string> output{};
    std::ifstream a_file(file_path);
    std::string str{};
    while (std::getline(a_file, str))
    {
        if (str.size() > 0)
            output.emplace_back(str);
    }
    a_file.close();
    return output;
}

/**
 * Get the current termios struct for STDIN. The output of this function is dependant on the state of STDIN.
 * @return the current termios struct
 */
termios get_curr_termios()
{
    termios term{};
    tcgetattr(STDIN_FILENO, &term);
    return term;
}

/**
 * Given a termios struct, return a new termios struct that is that termios struct set in raw mode
 * @param t the termios struct
 * @return a new termios struct that is set to raw mode
 */
termios make_raw_termios(const termios& t)
{
    termios new_t = t;
    new_t.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    new_t.c_oflag &= ~(OPOST);
    new_t.c_cflag |= (CS8);
    new_t.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    new_t.c_cc[VMIN] = 0;
    new_t.c_cc[VTIME] = 1;

    return new_t;
}

/**
 * Given a termios struct, set the STDIN stream attributes to that struct, and returns the original struct, unchanged
 * @param t the termios struct
 * @return returns the same termios struct, unmodified
 */
termios set_termios_attr(const termios& t)
{
    return tcsetattr(STDIN_FILENO, TCSAFLUSH, &t) == -1 ?
        throw std::exception() :
        t;
}

EditorConfig handle_new_file(const char* file_path)
{
    return {{0, 0}, 0, 0, open_file(file_path), true};
}

void hvim(const EditorConfig& editor_config)
{
    editor_refresh_screen(editor_config);
    if (editor_config.m_do_run)
        hvim(process_key_press(editor_config, editor_read_key()));
}

int main(int argc, char* argv[])
{

    const termios orig_term = get_curr_termios();
    const termios raw_term = make_raw_termios(orig_term);
    set_termios_attr(raw_term);

    if (argc > 1)
        hvim(handle_new_file(argv[1]));
    else
        hvim({{0, 0}, 0, 0, {}, true});

    set_termios_attr(orig_term);
    return 0;
}
