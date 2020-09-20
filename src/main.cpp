#include <iostream>
#include <cstdlib>
#include <array>
#include <vector>
#include <fstream>

#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <optional>

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
    int view_offset_x;
    int view_offset_y;
    termios orig_termios;
    std::vector<std::string> content{};

    enum Direction {
        LEFT,
        RIGHT,
        UP,
        DOWN,
        FORWARD,
        BACK
    };

    void move_cursor(Direction direction)
    {
        if (content.empty())
            return;
        switch (direction)
        {
            case LEFT:
                if (cursor.x != 0)
                    cursor.x--;
                break;
            case RIGHT:
                if (cursor.x != content.at(cursor.y).size() - 1)
                    cursor.x++;
                break;
            case UP:
                if (cursor.y != 0) {
                    cursor.y--;
                    if (cursor.x >= content.at(cursor.y).size())
                        cursor.x = content.at(cursor.y).size() - 1;
                }
                break;
            case DOWN:
                if (cursor.y != content.size() - 1) {
                    cursor.y++;
                    if (cursor.x >= content.at(cursor.y).size())
                        cursor.x = content.at(cursor.y).size() - 1;
                }
                break;
            case FORWARD:
                if (cursor.x != content.at(cursor.y).size() - 1)
                {
                    cursor.x++;
                } else if (cursor.y != content.size() - 1) {
                    cursor.x = 0;
                    cursor.y++;
                }
                break;
            case BACK:
                if (cursor.x != 0)
                {
                    cursor.x--;
                } else if (cursor.y != 0) {
                    cursor.x = content.at(--cursor.y).size() - 1;
                }
                break;
        }
    }
};

const std::string VERSION = "0.0.1";

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
    for (int y = 0; y < window_dimensions.height; y++)
    {
        if (y < editor_config.content.size())
        {
            output.append(editor_config.content.at(y + editor_config.view_offset_y));
        } else {
            output.append("~");
            if (editor_config.content.empty() && y == window_dimensions.height/3) {
                std::string welcome_no_padding = "Welcome to HVim -- Version " + VERSION;
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

void editor_refresh_screen(EditorConfig& editor_config)
{
    std::string output_str{};

    output_str.append("\x1b[?25l"); // Hide the cursor
    output_str.append("\x1b[H");    // Reset Cursor to top left
    output_str.append(editor_draw_rows(editor_config, get_window_size())); // Draw the content

    std::string move_cursor{"\x1b["};
    move_cursor += std::to_string(editor_config.cursor.y + 1) + ";" + std::to_string(editor_config.cursor.x + 1) + "H";
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
            editor_config.move_cursor(EditorConfig::LEFT);
            break;
        case ('j'):
            editor_config.move_cursor(EditorConfig::DOWN);
            break;
        case ('k'):
            editor_config.move_cursor(EditorConfig::UP);
            break;
        case ('l'):
            editor_config.move_cursor(EditorConfig::RIGHT);
            break;
        case (' '):
            editor_config.move_cursor(EditorConfig::FORWARD);
            break;
        case (127):
            editor_config.move_cursor(EditorConfig::BACK);
            break;
        default:
            break;
    }
    return true;
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

int main(int argc, char* argv[])
{
    EditorConfig editor_config;

    const termios orig_term = get_curr_termios();
    const termios raw_term = make_raw_termios(orig_term);
    set_termios_attr(raw_term);

    if (argc > 1)
        editor_config.content = open_file(argv[1]);

    bool run = true;
    while (run) {
        editor_refresh_screen(editor_config);
        run = process_key_press(editor_config);
    }
    set_termios_attr(orig_term);
    return 0;
}