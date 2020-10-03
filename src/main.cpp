#include <iostream>
#include <cstdlib>
#include <array>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <unistd.h>
#include <termios.h>
#include <optional>

#include "EditorConfig.h"
#include "util.h"

namespace Constants
{
    const std::string VERSION = "0.0.1";
    const std::unordered_map<char, Direction> nav_key_bindings = {
            {'h', LEFT},
            {'j', DOWN},
            {'k', UP},
            {'l', RIGHT},
            {' ', FORWARD},
            {127, BACK}
    };
    const std::unordered_map<char, Mode> switch_mode_key_bindings = {
            {'i', INSERT},
            {'v', VISUAL},
            {':', COMMAND},
            {'/', SEARCH}
    };
    enum Colour {
        BLACK, RED, GREEN, ORANGE, BLUE, PURPLE, CYAN, L_GRAY, D_GRAY, L_RED, L_GREEN, YELLOW, L_BLUE, L_PURPLE, L_CYAN,
        WHITE
    };
    const std::unordered_map<Colour, std::string> colour_escape_sequences = {
            {BLACK, "\x1b[0;30m"},
            {RED, "\x1b[0;31m"},
            {GREEN, "\x1b[0;32m"},
            {ORANGE, "\x1b[0;33m"},
            {BLUE, "\x1b[0;34m"},
            {PURPLE, "\x1b[0;35m"},
            {CYAN, "\x1b[0;36m"},
            {L_GRAY, "\x1b[0;37m"},
            {D_GRAY, "\x1b[0;90m"},
            {L_RED, "\x1b[0;91m"},
            {L_GREEN, "\x1b[0;92m"},
            {YELLOW, "\x1b[0;93m"},
            {L_BLUE, "\x1b[0;94m"},
            {L_PURPLE, "\x1b[0;95m"},
            {L_CYAN, "\x1b[0;96m"},
            {WHITE, "\x1b[0;97m"}
    };
};

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

auto bind_highlight(const std::unordered_map<Constants::Colour, std::string>& colour_map)
{
    return [&colour_map](const std::string& string, const Constants::Colour&& colour)->std::string{
        return colour_map.at(colour) + string + colour_map.at(Constants::Colour::BLACK);
    };
}

const auto highlight = bind_highlight(Constants::colour_escape_sequences);

std::string editor_draw_rows(const EditorConfig& editor_config, const WindowSize& window_dimensions)
{
    std::string output{};
    for (int y = 0; y < window_dimensions.height; y++)
    {
        if (y >= window_dimensions.height - 2) {
            if (y >= window_dimensions.height - 1 && editor_config.m_mode == Mode::INSERT)
                output.append("--INSERT--");
        }
        else if (y + editor_config.m_view_offset.y < editor_config.m_content.size())
        {
            output.append(
                highlight(
                    std::string(num_digits(editor_config.m_content.size()) - num_digits(y + editor_config.m_view_offset.y + 1), ' ') +
                    std::to_string(y + editor_config.m_view_offset.y + 1) + ' ',
                    Constants::Colour::ORANGE) +
                highlight(editor_config.m_content.at(y + editor_config.m_view_offset.y), Constants::Colour::PURPLE)
            );
        } else {
            output.append("\x1b[33m~\x1b[0;11m");
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
    move_cursor += std::to_string(editor_config.m_cursor.y - editor_config.m_view_offset.y + 1) + ";" +
            std::to_string(editor_config.m_cursor.x - editor_config.m_view_offset.x + 2 +
            num_digits(editor_config.m_content.size())) + "H";
    output_str.append(move_cursor);

    output_str.append("\x1b[?25h"); // Show the cursor

    write(STDOUT_FILENO, output_str.c_str(), output_str.length());
}

Direction get_move_direction(const char c)
{
    return Constants::nav_key_bindings.at(c);
}

/**
 * Given an unordered_map of characters to Directions, bind said map to a function that, given a character,
 * returns whether that character is a key in the original unordered_map
 * @param key_map
 * @return lambda for checking if a character is in the unordered_map
 */
template<typename MappedType>
auto is_key(const std::unordered_map<char, MappedType>& key_map)
{
    return [&key_map](const char& c){ return key_map.contains(c); };
}

const auto is_nav_key = is_key(Constants::nav_key_bindings);
const auto is_switch_key = is_key(Constants::switch_mode_key_bindings);

EditorConfig process_navigate_key_press(const EditorConfig& e, const char c)
{
    if (is_switch_key(c))
        return {e.m_cursor, e.m_view_offset, e.m_content, e.m_do_run, Constants::switch_mode_key_bindings.at(c)};
    if (is_nav_key(c))
        return (
            [&e](const auto& new_cursor) -> EditorConfig
            {
                return {new_cursor, update_offset(e.m_view_offset, new_cursor), e.m_content, e.m_do_run};
            }
        )(move_cursor(e, get_move_direction(c)));
    if (c == 'q') return {e.m_cursor, e.m_view_offset, e.m_content, false};
    return e;
}

EditorConfig process_insert_key_press(const EditorConfig& e, const char c)
{
    if (c == 27)
        return {e.m_cursor, e.m_view_offset, e.m_content, e.m_do_run, Mode::NAVIGATE};
    return e;
}

EditorConfig process_key_press(const EditorConfig& e, const char c)
{
    switch(e.m_mode)
    {
        case NAVIGATE: {
            return process_navigate_key_press(e, c);
        }
        case INSERT: {
            return process_insert_key_press(e, c);
        }
    }
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
    return {{0, 0}, {0, 0}, open_file(file_path), true};
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
        hvim({{0, 0}, {0, 0}, {}, true});

    set_termios_attr(orig_term);
    return 0;
}
