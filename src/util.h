//
// Created by Harrison Marshall on 26/9/20.
//
#ifndef HVIMSRC_UTIL_H
#define HVIMSRC_UTIL_H

#include <array>
#include <sys/ioctl.h>

struct WindowSize
{
    int width;
    int height;
};


/**
 * Given an int n, return the number of digits in n
 * @param n The number to count the digits of
 * @return The number of digits in n
 */
int num_digits(int n)
{
    int digits = 0;
    if (n < 0) digits = 1;
    while (n)
    {
        n /= 10;
        digits++;
    }
    return digits;
}

/**
 * Return the location of the cursor in the terminal window
 * @return The location of the cursor in the terminal window
 */
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

/**
 * Get the size of the terminal window
 * @return
 */
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

#endif