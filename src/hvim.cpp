#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <csignal>

#include "term.hpp"

void handleResize(int v)
{
    std::cout << "Resize!" << '\n';
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

int main(int argc, char* argv[])
{
    signal(SIGWINCH, handleResize);
    Term term{};

    auto content = open_file((argc == 2) ? argv[1] : "");

    bool do_run = true;
    char c{};
    size_t height{};
    while (do_run)
    {
        height = term.getHeight();
        for (auto it = std::begin(content); it != std::end(content) && it != std::begin(content) + height; it++)
        {
            term.draw(it - std::begin(content), 0, (*it).c_str());
        }
        c = term.readChar();
        if (c == 'q') do_run = false;
        term.refresh();
    }
}
