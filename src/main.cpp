#include <iostream>
#include <cstdlib>


#include <cctype>
#include <unistd.h>
#include <termios.h>

inline void disableRawMode(const termios& orig_termios)
{
    std::cout << "disabling raw mode" << '\n';
    std::cout << tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode(const termios& orig_termios)
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

void editor_clear_screen()
{
    write(STDOUT_FILENO, "\x1b[2J", 4); // Clear Screen
    write(STDOUT_FILENO, "\x1b[H", 3);  // Reset Cursor to top left
}

bool process_key_press()
{
    char c = editor_read_key();
    editor_clear_screen();

    switch (c)
    {
        case ('q'):
            return false;
        default:
            write(STDOUT_FILENO, &c, 1);
    }
    return true;
}

int main()
{
    termios orig_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);
    enableRawMode(orig_termios);
    while ( process_key_press() ) {}
    disableRawMode(orig_termios);
    return 0;
}