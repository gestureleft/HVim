#include <iostream>
#include "term.hpp"

int main()
{
    Term term{};
    term.write(0, 0, "Hello there");
    term.refresh();
    std::cout << "Hello from src!\n";
}
