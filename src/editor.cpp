#include "editor.hpp"
#include "globals.hpp"

void push_to_editor(std::string s)
{
    if (e.lines[e.cursor_y].empty())
        e.lines[e.cursor_y] = s;
    else
        e.lines[e.cursor_y].insert(e.cursor_x, s);
    e.cursor_x += s.length();
}

bool read_from_file(std::string &file_name)
{

    // Validate the filename
    if (file_name.empty())
    {
        std::cerr << "Invalid file name" << std::endl;
        return false;
    }

    // Open the file
    std::ifstream file(file_name);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << file_name << std::endl;
        return false;
    }

    // Read each line from the file and add it to e.lines vector
    std::string line;
    int space_count = 0;
    while (std::getline(file, line))
    {
        e.lines.push_back(line);
        for (char x : line)
        {
            if (x == ' ')
                space_count++;
            else
                break;
        }
        e.tab_offset_vec.push_back(TAB.length() * (space_count / TAB.length()));
        space_count = 0;
    }

    if (e.lines.size() > 0 && e.lines[0].empty())
    {
        if (!e.lines.empty())
        {
            e.lines.erase(e.lines.begin());
        }
    }

    // Close the file
    file.close();

    return true;
}
