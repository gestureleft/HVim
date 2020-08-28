#include <iostream>
#include <cstdlib>

#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

struct EditorConfig
{
    termios orig_termios;
};

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

std::pair<int,int> get_window_size()
{
    winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
    {
        throw 1;
    } else {
        return {ws.ws_row, ws.ws_col};
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

void editor_draw_rows()
{
    for (int y = 0; y < get_window_size().first; y++)
    {
        write(STDOUT_FILENO, "~\r\n", 3);
    }
}

void editor_refresh_screen()
{
    write(STDOUT_FILENO, "\x1b[2J", 4); // Clear Screen
    write(STDOUT_FILENO, "\x1b[H", 3);  // Reset Cursor to top left
    editor_draw_rows();
    write(STDOUT_FILENO, "\x1b[1;2H", 6);
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

    bool run = true;
    while (run) {
        editor_refresh_screen();
        run = process_key_press();
    }
    disable_raw_mode(editor_config.orig_termios);
    return 0;
}