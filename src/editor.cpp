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

void enter_to_editor()
{
    bool marked = true;
    std::vector<char> indent_keys = {':', '{', '[', '('};
    std::string current_line = e.lines[e.cursor_y];
    std::string new_line = current_line.substr(e.cursor_x);
    e.lines[e.cursor_y].erase(e.cursor_x);
    e.cursor_y++;
    if (!e.lines[e.cursor_y - 1].empty())
    {
        for (char &char_x : indent_keys)
        {
            if (e.lines[e.cursor_y - 1][e.cursor_x - 1] == char_x)
            {
                e.tab_offset_vec.insert(e.tab_offset_vec.begin() + e.cursor_y, TAB.length() + e.tab_offset_vec[e.cursor_y - 1]);
                marked = false;
            }
        }
    }
    if (marked)
    {
        e.tab_offset_vec.insert(e.tab_offset_vec.begin() + e.cursor_y, e.tab_offset_vec[e.cursor_y - 1]);
    }
    e.cursor_x = 0 + e.tab_offset_vec[e.cursor_y];
    new_line = (TAB * e.tab_offset_vec[e.cursor_y]) + new_line;
    e.lines.insert(e.lines.begin() + e.cursor_y, new_line);
}

void editor_up()
{
    if (e.cursor_y > 0)
    {
        if (e.cursor_x > e.lines[e.cursor_y - 1].length())
            e.cursor_x = e.lines[e.cursor_y - 1].length();
        e.cursor_y--;
    }
}

void editor_left()
{
    if (e.cursor_x > 0)
    {
        if (CTRL_PRESSED)
        {
            // Handle delimiters
            if (std::find(delimiters.begin(), delimiters.end(), e.lines[e.cursor_y][e.cursor_x - 1]) != delimiters.end())
            {
                while (e.cursor_x > 0 && std::find(delimiters.begin(), delimiters.end(), e.lines[e.cursor_y][e.cursor_x - 1]) != delimiters.end())
                {
                    e.cursor_x--;
                }
            }
            else
            {
                // Skip whitespace first
                while (e.cursor_x > 0 && std::isspace(e.lines[e.cursor_y][e.cursor_x - 1]))
                {
                    e.cursor_x--;
                }
                // Then skip non-whitespace characters
                while (e.cursor_x > 0 && !std::isspace(e.lines[e.cursor_y][e.cursor_x - 1]) && std::find(delimiters.begin(), delimiters.end(), e.lines[e.cursor_y][e.cursor_x - 1]) == delimiters.end())
                {
                    e.cursor_x--;
                }
            }
        }
        else
        {
            e.cursor_x--;
        }
    }
    else if (e.cursor_y > 0)
    {
        // If the user presses left on a space such as
        // hello world!
        // |
        // then the cursor goes : hello world!|
        e.cursor_y--;
        e.cursor_x = e.lines[e.cursor_y].length();
    }
}

void editor_right()
{
    if (e.cursor_x < e.lines[e.cursor_y].length())
    {
        if (CTRL_PRESSED)
        {
            // Handle delimiters
            if (std::find(delimiters.begin(), delimiters.end(), e.lines[e.cursor_y][e.cursor_x]) != delimiters.end())
            {
                while (e.cursor_x < e.lines[e.cursor_y].length() && std::find(delimiters.begin(), delimiters.end(), e.lines[e.cursor_y][e.cursor_x]) != delimiters.end())
                {
                    e.cursor_x++;
                }
            }
            else
            {
                // Skip whitespace first
                while (e.cursor_x < e.lines[e.cursor_y].length() && std::isspace(e.lines[e.cursor_y][e.cursor_x]))
                {
                    e.cursor_x++;
                }

                // Then skip non-whitespace characters
                while (e.cursor_x < e.lines[e.cursor_y].length() && !std::isspace(e.lines[e.cursor_y][e.cursor_x]) && std::find(delimiters.begin(), delimiters.end(), e.lines[e.cursor_y][e.cursor_x]) == delimiters.end())
                {
                    e.cursor_x++;
                }
            }
        }
        else
        {
            e.cursor_x++;
        }
    }
    else if (e.cursor_y < e.lines.size() - 1)
    {
        e.cursor_y++;
        e.cursor_x = 0;
    }
}

void editor_down()
{
    if (e.cursor_y < e.lines.size() - 1)
    {
        if (e.cursor_x > e.lines[e.cursor_y + 1].length())
            e.cursor_x = e.lines[e.cursor_y + 1].length();
        e.cursor_y++;
    }
}

void editor_backspace()
{
    if (e.cursor_x == 0 && e.cursor_y > 0)
    {
        int prev_line_length = e.lines[e.cursor_y - 1].length();
        e.lines[e.cursor_y - 1] += e.lines[e.cursor_y];
        e.lines.erase(e.lines.begin() + e.cursor_y);
        e.cursor_y--;
        e.cursor_x = prev_line_length;
    }
    else if (CTRL_PRESSED)
    {
        // Delete to the nearest space or bracket
        while (e.cursor_x > 0 && !std::isspace(e.lines[e.cursor_y][e.cursor_x - 1]) &&
               e.lines[e.cursor_y][e.cursor_x - 1] != '(' && e.lines[e.cursor_y][e.cursor_x - 1] != '[' &&
               e.lines[e.cursor_y][e.cursor_x - 1] != '{')
        {
            e.lines[e.cursor_y].erase(e.cursor_x - 1, 1);
            e.cursor_x--;
        }
        if (std::isspace(e.lines[e.cursor_y][e.cursor_x - 1]))
        {
            while (e.cursor_x > 0 && std::isspace(e.lines[e.cursor_y][e.cursor_x - 1]))
            {
                e.lines[e.cursor_y].erase(e.cursor_x - 1, 1);
                e.cursor_x--;
            }
        }
    }
    else if (e.cursor_x > 0)
    {
        if (e.cursor_x >= TAB.length() && e.lines[e.cursor_y].substr(e.cursor_x - TAB.length(), TAB.length()) == TAB)
        {
            // Delete the whole tab
            e.lines[e.cursor_y].erase(e.cursor_x - TAB.length(), TAB.length());
            e.tab_offset_vec[e.cursor_y] -= TAB.length();
            e.cursor_x -= TAB.length();
        }
        else if (std::isspace(e.lines[e.cursor_y][e.cursor_x - 1]))
        {
            // Delete connected spaces
            while (e.cursor_x > 0 && std::isspace(e.lines[e.cursor_y][e.cursor_x - 1]))
            {
                e.lines[e.cursor_y].erase(e.cursor_x - 1, 1);
                e.cursor_x--;
            }
        }
        else
        {
            // Delete a single character
            e.lines[e.cursor_y].erase(e.cursor_x - 1, 1);
            e.cursor_x--;
        }
    }
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
