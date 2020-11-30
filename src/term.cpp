#include <termios.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>

#include "term.hpp"

Term::Term()
{
    tcgetattr(STDIN_FILENO, &original_terminal_settings);
    termios terminal = original_terminal_settings;
    terminal.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    terminal.c_oflag &= ~(OPOST);
    terminal.c_cflag |= (CS8);
    terminal.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    terminal.c_cc[VMIN] = 0;
    terminal.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminal);
}

Term::~Term()
{
    // Clear screen at exit:
    //buffer = "\x1b[2J";
    //write(STDOUT_FILENO, buffer.c_str(), buffer.length());
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal_settings);
}

size_t Term::getWidth()
{
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    return ws.ws_col;
}

size_t Term::getHeight()
{
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    return ws.ws_row;
}

void Term::draw(size_t x, size_t y, const char* content)
{
    buffer += "\x1b[" + std::to_string(x) + ";" + std::to_string(y) + "H";
    buffer.append(content);
}

void Term::draw(size_t x, size_t y, const char* content, Colour colour)
{
    buffer += "\x1b[38;5;" + std::to_string(static_cast<int>(colour)) + "m";
    draw(x,y,content);
    buffer += "\x1b[38;5;" + std::to_string(static_cast<int>(default_colour)) + "m";
}

void Term::refresh()
{
    buffer = "\x1b[2J" + buffer;
    write(STDOUT_FILENO, buffer.c_str(), buffer.length());
    buffer = "";
}

char Term::readChar()
{
    char c{};
    while (read(STDIN_FILENO, &c, 1) != 1) {}
    return c;
}

