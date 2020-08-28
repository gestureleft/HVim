#include <iostream>
#include <cstdlib>


#include <cctype>
#include <unistd.h>
#include <termios.h>

void disableRawMode(const termios& orig_termios)
{
    std::cout << "Exiting raw mode" << '\n';
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

int main()
{
    termios orig_termios;
    enableRawMode(orig_termios);
    char c = '\0';
    while (1)
    {
        if (read(STDIN_FILENO, &c, 1) != 0 && errno == EAGAIN) break;

        if (iscntrl(c))
            std::cout << static_cast<int>(c) << '\r' << '\n';
        else
            std::cout << '(' << static_cast<int>(c) << "):" << c << '\r' << '\n';

        if (c == 'q') break;
    }
    disableRawMode(orig_termios);
    return 0;
}