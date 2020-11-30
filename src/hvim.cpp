#include <iostream>
#include <string>
#include <csignal>

#include "term.hpp"

void handleResize(int v)
{
    std::cout << "Resize!" << '\n';
}

int main()
{
    signal(SIGWINCH, handleResize);
    Term term{};
    term.draw(0, 6, "hmm there", Term::Colour::Red);
    term.draw(2, 7, "hmm there");
    std::string test_string = "Bottom Right!";
    const size_t width = term.getWidth();
    const size_t height = term.getHeight();
    term.draw(height, width - test_string.length(), test_string.c_str(), Term::Colour::Blue);
    term.refresh();

    bool do_run = true;
    char c{};
    std::string label{};
    while (do_run)
    {
        label = "Key pressed: ";
        c = term.readChar();
        if (c == 'q') do_run = false;
        term.draw(0,0, (label + c).c_str());
        term.refresh();
    }
}
