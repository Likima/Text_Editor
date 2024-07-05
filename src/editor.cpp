#include "editor.hpp"
#include "globals.hpp"

bool CTRL_PRESSED = false;

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
        CTRL_PRESSED = false;
    }
    break;
    case (GLFW_PRESS):
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
            std::string current_line = e.lines[e.cursor_y];
            std::string new_line = current_line.substr(e.cursor_x);
            e.lines[e.cursor_y].erase(e.cursor_x);
            e.cursor_y++;
            e.cursor_x = 0;
            e.lines.insert(e.lines.begin() + e.cursor_y, new_line);
        }
        break;
        case (GLFW_KEY_TAB):
        {
            push_to_editor("   ");
        }
        break;
        case (GLFW_KEY_UP):
        {
            if(e.cursor_y > 0) {
                if(e.cursor_x > e.lines[e.cursor_y - 1].length())
                    e.cursor_x = e.lines[e.cursor_y - 1].length();
                e.cursor_y--;
            }
        }
        break;
        case (GLFW_KEY_DOWN):
        {
            if(e.cursor_y < e.lines.size() - 1) {
                if(e.cursor_x > e.lines[e.cursor_y + 1].length())
                    e.cursor_x = e.lines[e.cursor_y + 1].length();
                e.cursor_y++;
            }
        }
        break;
        case (GLFW_KEY_LEFT):
        {
            if (e.cursor_x > 0) {
                e.cursor_x--;
            }
        }
        break;
        case (GLFW_KEY_RIGHT):
        {
            if (e.cursor_x < e.lines[e.cursor_y].length())
                e.cursor_x++;
        }
        break;
        case (GLFW_KEY_LEFT_CONTROL):
        case (GLFW_KEY_RIGHT_CONTROL): {
            CTRL_PRESSED = true;
            std::cout<<"CTRL PRESSED"<<std::endl;
        }
        break;
        case (GLFW_KEY_BACKSPACE): {
            if(e.cursor_x > 0) {
                e.lines[e.cursor_y].erase(e.cursor_x - 1, 1);
                e.cursor_x--;
            }
            if(e.cursor_x == 0 && e.cursor_y > 0) {
                int prev_line_length = e.lines[e.cursor_y - 1].length();
                e.lines[e.cursor_y - 1] += e.lines[e.cursor_y];
                e.lines.erase(e.lines.begin() + e.cursor_y);
                e.cursor_y--;
                e.cursor_x = prev_line_length;
            }
        }
        break;
        default:
            std::cout << key << std::endl;
        }
    }
    break;
    case (GLFW_REPEAT): // Handle holding keys
    {
        switch (key)
        {
        case (GLFW_KEY_UP):
        {
            if(e.cursor_y > 0) {
                if(e.cursor_x > e.lines[e.cursor_y - 1].length())
                    e.cursor_x = e.lines[e.cursor_y - 1].length();
                e.cursor_y--;
            }
        }
        break;
        case (GLFW_KEY_DOWN):
        {
            if(e.cursor_y < e.lines.size() - 1) {
                if(e.cursor_x > e.lines[e.cursor_y + 1].length())
                    e.cursor_x = e.lines[e.cursor_y + 1].length();
                e.cursor_y++;
            }
        }
        break;
        case (GLFW_KEY_LEFT):
        {
            if (e.cursor_x > 0) {
                e.cursor_x--;
            }
        }
        break;
        case (GLFW_KEY_RIGHT):
        {
            if (e.cursor_x < e.lines[e.cursor_y].length())
                e.cursor_x++;
        }
        break;
        case (GLFW_KEY_BACKSPACE): {
            if(e.cursor_x > 0) {
                e.lines[e.cursor_y].erase(e.cursor_x - 1, 1);
                e.cursor_x--;
            }
        }
        break;
        }
    }
    break;
    }
    if(CTRL_PRESSED && action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_S)
            save_to_file();
        else if (key == GLFW_KEY_BACKSPACE) {
            // Delete to the nearest space or bracket
            while (e.cursor_x > 0 && !std::isspace(e.lines[e.cursor_y][e.cursor_x - 1]) && e.lines[e.cursor_y][e.cursor_x - 1] != '(' && e.lines[e.cursor_y][e.cursor_x - 1] != '[' && e.lines[e.cursor_y][e.cursor_x - 1] != '{') {
                e.lines[e.cursor_y].erase(e.cursor_x - 1, 1);
                e.cursor_x--;
            }
        }
    }
}

void save_to_file(int added) {
    if (save_file.empty()) {
        save_file = "GenericFile" + std::to_string(added) + ".txt";
        std::ifstream file(save_file.c_str());
        if (file.is_open()) {
            file.close();
            save_file.clear();
            save_to_file(added + 1);
        } else {
            std::ofstream file(save_file);
            if (file.is_open())
                file.close();
            else
                return;
        }
    }
    std::ofstream file(save_file.c_str());
    if (file.is_open()) {
        for (const std::string& line : e.lines) {
            file << line << std::endl;
        }
        file.close();
    }
}

void char_callback(GLFWwindow *window, unsigned int codepoint)
{
    std::string character(1, static_cast<char>(codepoint));
    switch(CTRL_PRESSED) {
        case(false): {           
            // std::cout << "Character input: " << character << " : " << std::endl;
            push_to_editor(character);
        }
        break;
        default: {
            if(character == "s") {
                save_to_file();
            }
        }
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

void renderText(GLuint &s, std::vector<std::string> text, float x, float y, float scale, glm::vec3 color)
{
    glUniform3f(glGetUniformLocation(s, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for (int i = 0; i < text.size(); i++)
    {
        float xPos = x; // Reset x position for each line
        for (auto c = text[i].begin(); c != text[i].end(); c++)
        {
            Character ch = Characters[*c];

            float xpos = xPos + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale - (i * FONT_SIZE * scale); // Adjust y position
            // std::cout << xpos << " " << ypos << " " << (ch.Size.y - ch.Bearing.y) << std::endl;

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

void renderCursor(GLuint &s, float x, float y, float scale, glm::vec3 color)
{
    GLchar u_line = '|';
    Character ch = Characters[u_line];
    glUniform3f(glGetUniformLocation(s, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    float xpos = x + (ch.Bearing.x * scale);
    for(int i = 0; i < e.cursor_x; i++) {
        xpos += (ch.Advance >> 6) * scale;
    }
    float ypos = y - (ch.Size.y - ch.Bearing.y) * scale - (FONT_SIZE * scale * (e.cursor_y)); // Adjust y position    

    float x_Offset = -1 * ch.Bearing.x; // for the | character

    xpos += x_Offset;

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