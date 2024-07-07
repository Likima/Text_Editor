#include "editor.hpp"
#include "globals.hpp"

bool CTRL_PRESSED = false;

const std::string TAB = "    ";
const std::vector<char> delimiters = {'"', '\'', '(', ')', '{', '}', '[', ']', '/', ':', ';', '<', '>'};

void push_to_editor(std::string s)
{
    if (e.lines[e.cursor_y].empty())
        e.lines[e.cursor_y] = s;
    else
        e.lines[e.cursor_y].insert(e.cursor_x, s);
    e.cursor_x += s.length();
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
        break;
        case (GLFW_KEY_TAB):
        {
            push_to_editor(TAB);
            e.tab_offset_vec[e.cursor_y] += 1;
        }
        break;
        case (GLFW_KEY_UP):
        {
            if (e.cursor_y > 0)
            {
                if (e.cursor_x > e.lines[e.cursor_y - 1].length())
                    e.cursor_x = e.lines[e.cursor_y - 1].length();
                e.cursor_y--;
            }
        }
        break;
        case (GLFW_KEY_DOWN):
        {
            if (e.cursor_y < e.lines.size() - 1)
            {
                if (e.cursor_x > e.lines[e.cursor_y + 1].length())
                    e.cursor_x = e.lines[e.cursor_y + 1].length();
                e.cursor_y++;
            }
        }
        break;
        case (GLFW_KEY_LEFT):
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
        break;
        case (GLFW_KEY_RIGHT):
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
        break;
        case (GLFW_KEY_LEFT_CONTROL):
        case (GLFW_KEY_RIGHT_CONTROL):
        {
            CTRL_PRESSED = true;
        }
        break;
        case (GLFW_KEY_BACKSPACE):
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

void renderText(GLuint &s, std::vector<std::string> text, float x, float y, float scale)
{
    bool isKeyword = false;
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for (int i = 0; i < text.size(); i++)
    {
        float xPos = x; // Reset x position for each line
        std::vector<Token> tokens = tokenize(text[i]);
        for (auto &token : tokens)
        {
            // printToken(token);
            //  Set the color based on token type
            glm::vec3 color;
            switch (token.type)
            {
            case KEYWORD:
                color = glm::vec3(0.0f, 1.0f, 0.0f);
                isKeyword = true;
                break; // Green
            case IDENTIFIER:
                color = glm::vec3(1.0f, 1.0f, 1.0f);
                break; // White
            case NUMBER:
                color = glm::vec3(1.0f, 1.0f, 0.0f);
                break; // Yellow
            case STRING:
                color = glm::vec3(1.0f, 0.0f, 1.0f);
                break; // Magenta
            case OPERATOR:
                color = glm::vec3(0.0f, 0.0f, 1.0f);
                break; // Blue
            case COMMENT:
                color = glm::vec3(0.5f, 0.5f, 0.5f);
                break; // Gray
            default:
                color = glm::vec3(1.0f, 1.0f, 1.0f);
                break; // Default to white
            }
            glUniform1i(glGetUniformLocation(s, "isKeyword"), isKeyword);
            isKeyword = false;
            glUniform3f(glGetUniformLocation(s, "textColor"), color.x, color.y, color.z);

            for (auto c = token.text.begin(); c != token.text.end(); c++)
            {
                Character ch = Characters[*c];

                float xpos = xPos + ch.Bearing.x * scale;
                float ypos = y - (ch.Size.y - ch.Bearing.y) * scale - (i * FONT_SIZE * scale); // Adjust y position

                float w = ch.Size.x * scale;
                float h = ch.Size.y * scale;

                float vertices[6][4] = {
                    {xpos, ypos + h, 0.0f, 0.0f},
                    {xpos, ypos, 0.0f, 1.0f},
                    {xpos + w, ypos, 1.0f, 1.0f},

                    {xpos, ypos + h, 0.0f, 0.0f},
                    {xpos + w, ypos, 1.0f, 1.0f},
                    {xpos + w, ypos + h, 1.0f, 0.0f}};

                xPos += (ch.Advance >> 6) * scale; // Advance cursor to next glyph
                glBindTexture(GL_TEXTURE_2D, ch.TextureID);
                glBindVertexArray(VAO);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void renderLineNumbers(GLuint &s, float x, float y, glm::vec3 color)
{
    glUniform3f(glGetUniformLocation(s, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    float scale = 1.0f; // Adjust scale as necessary
    std::string converted;
    for (int i = 1; i <= e.lines.size(); i++)
    {
        converted = std::to_string(i);

        float xPos = x; // Start position for line number
        for (char c : converted)
        {
            Character ch = Characters[c];

            float xpos = xPos + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale - ((i - 1) * FONT_SIZE * scale); // Adjust y position

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;

            float vertices[6][4] = {
                {xpos, ypos + h, 0.0f, 0.0f},
                {xpos, ypos, 0.0f, 1.0f},
                {xpos + w, ypos, 1.0f, 1.0f},

                {xpos, ypos + h, 0.0f, 0.0f},
                {xpos + w, ypos, 1.0f, 1.0f},
                {xpos + w, ypos + h, 1.0f, 0.0f}};

            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            xPos += (ch.Advance >> 6) * scale; // Advance cursor to next glyph
        }
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

glm::vec2 currentCameraPos(0.0f, 0.0f);
glm::vec2 targetCameraPos(0.0f, 0.0f);
float interpolationFactor = 0.1f; // Adjust for smoothness

// Delete this function to remove the motion sickness
void updateProjection(GLuint program, int xpos, int ypos)
{
    if (!scrolling)
        return;

    float targetOffsetX = (xpos - SCREEN_WIDTH / 2.0f) + SCREEN_WIDTH / 6.0f;
    float targetOffsetY = ypos - SCREEN_HEIGHT / 2.0f; // Y-axis might be inverted

    targetCameraPos = glm::vec2(targetOffsetX, targetOffsetY);

    // Interpolate between the current and target positions
    currentCameraPos = glm::mix(currentCameraPos, targetCameraPos, interpolationFactor);

    glm::mat4 projection = glm::ortho(
        currentCameraPos.x, currentCameraPos.x + static_cast<float>(SCREEN_WIDTH),
        currentCameraPos.y, currentCameraPos.y + static_cast<float>(SCREEN_HEIGHT));

    glUseProgram(program);
    GLuint mLocation = glGetUniformLocation(program, "projection");
    if (mLocation == -1)
    {
        std::cerr << "Failed to find uniform location for 'projection'" << std::endl;
        return;
    }
    glUniformMatrix4fv(mLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

void renderCursor(GLuint &s, float x, float y, float scale, glm::vec3 color)
{
    GLchar u_line = '|';
    Character ch = Characters[u_line];
    glUniform3f(glGetUniformLocation(s, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    float xpos = x + (ch.Bearing.x * scale);
    for (int i = 0; i < e.cursor_x; i++)
    {
        xpos += (ch.Advance >> 6) * scale;
    }
    float ypos = y - (ch.Size.y - ch.Bearing.y) * scale - (FONT_SIZE * scale * (e.cursor_y)); // Adjust y position

    float x_Offset = -1 * ch.Bearing.x; // for the | character

    xpos += x_Offset;

    updateProjection(program, xpos, ypos);

    // Define the vertices of the cursor
    float w = ch.Size.x * scale;
    float h = ch.Size.y * scale;

    float vertices[6][4] = {
        {xpos, ypos + h, 0.0f, 0.0f},
        {xpos, ypos, 0.0f, 1.0f},
        {xpos + w, ypos, 1.0f, 1.0f},
        {xpos, ypos + h, 0.0f, 0.0f},
        {xpos + w, ypos, 1.0f, 1.0f},
        {xpos + w, ypos + h, 1.0f, 0.0f}};

    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
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
