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
float interpolationFactor = 0.01f; // Adjust for smoothness

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
