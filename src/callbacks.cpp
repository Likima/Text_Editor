#include "globals.hpp"
#include "editor.hpp"
#include "callbacks.hpp"

bool CTRL_PRESSED = false;

void save_to_file(int added)
{
    if (save_file.empty())
    {
        save_file = "GenericFile" + std::to_string(added) + ".txt";
        std::ifstream file(save_file.c_str());
        if (file.is_open())
        {
            file.close();
            save_file.clear();
            save_to_file(added + 1);
        }
        else
        {
            std::ofstream file(save_file);
            if (file.is_open())
                file.close();
            else
                return;
        }
    }
    std::ofstream file(save_file.c_str());
    if (file.is_open())
    {
        for (const std::string &line : e.lines)
        {
            file << line << std::endl;
        }
        file.close();
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    switch (action)
    {
    case (GLFW_RELEASE):
    {
        if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)
            CTRL_PRESSED = false;
    }
    break;
    case (GLFW_PRESS):
    case (GLFW_REPEAT):
    {
        switch (key)
        {
        case (GLFW_KEY_ESCAPE):
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
            save_to_file();
        }
        break;
        case (GLFW_KEY_ENTER):
        {
            enter_to_editor();
        }
        break;
        case (GLFW_KEY_TAB):
        {
            push_to_editor(TAB);
            e.tab_offset_vec[e.cursor_y] += 1;
        }
        break;
        case (GLFW_KEY_UP):
        {
            editor_up();
        }
        break;
        case (GLFW_KEY_DOWN):
        {
            editor_down();
        }
        break;
        case (GLFW_KEY_LEFT):
        {
            editor_left();
        }
        break;
        case (GLFW_KEY_RIGHT):
        {
            editor_right();
        }
        break;
        case (GLFW_KEY_LEFT_CONTROL):
        case (GLFW_KEY_RIGHT_CONTROL):
        {
            CTRL_PRESSED = true;
        }
        break;
        case (GLFW_KEY_BACKSPACE):
        {
            editor_backspace();
        }
        break;
        case (GLFW_KEY_HOME):
        {
            for (int i = 0; i < e.lines[e.cursor_y].length(); i++)
            {
                if (!std::isspace(e.lines[e.cursor_y][i]))
                {
                    e.cursor_x = i;
                    break;
                }
            }
        }
        break;
        case (GLFW_KEY_END):
        {
            for (int i = e.lines[e.cursor_y].length() - 1; i >= 0; i--)
            {
                if (!std::isspace(e.lines[e.cursor_y][i]))
                {
                    e.cursor_x = i + 1;
                    break;
                }
            }
        }
        break;
        }
    }
    break;
    }
    if (CTRL_PRESSED && action == GLFW_PRESS || action == GLFW_REPEAT) // means user has press CTRL + something
    {
        if (key == GLFW_KEY_S)
            save_to_file();
    }
}

void char_callback(GLFWwindow *window, unsigned int codepoint)
{
    std::string character(1, static_cast<char>(codepoint));
    switch (CTRL_PRESSED)
    {
    case (false):
    {
        push_to_editor(character);
    }
    break;
    default:
    {
        if (character == "s")
        {
            save_to_file();
        }
    }
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    if (yoffset == -1 && e.cursor_y < e.lines.size())
    {
        e.cursor_y++;
        if (e.cursor_x > e.lines[e.cursor_y + 1].length())
            e.cursor_x = e.lines[e.cursor_y + 1].length();
    }
    else if (yoffset == 1 && e.cursor_y != 0)
    {
        e.cursor_y--;
        if (e.cursor_x > e.lines[e.cursor_y - 1].length())
            e.cursor_x = e.lines[e.cursor_y - 1].length();
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // Adjust the viewport when the window is resized
    glViewport(0, 0, width, height);

    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;

    // Update the projection matrix
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}
