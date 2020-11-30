#include <iostream>
#include <string>
#include "term.hpp"

int main()
{
    Term term{};
    term.draw(0, 6, "hmm there", Term::Colour::Red);
    term.draw(2, 7, "hmm there");
    std::string test_string = "Bottom Right!";
    const size_t width = term.getWidth();
    const size_t height = term.getHeight();
    term.draw(height, width - test_string.length(), test_string.c_str(), Term::Colour::Blue);
    term.refresh();
}
