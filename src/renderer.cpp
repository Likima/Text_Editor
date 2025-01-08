#include "globals.hpp"
#include "renderer.hpp"

struct lineNumberData
{
    std::string converted;
    std::vector<float> vertices;
    std::vector<GLuint> textures;
    std::vector<size_t> indices;
};

struct textData
{
    std::vector<float> vertices;
    std::vector<GLuint> textures;
    std::vector<size_t> indices;
    std::vector<glm::vec3> colors; // Store colors for each character batch
};

lineNumberData lnd;
textData td;

void renderText(GLuint &s, std::vector<std::string> text, float x, float y, float scale)
{
    glUseProgram(s);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    if (text_changed)
    {
        std::vector<float> vertices;
        std::vector<GLuint> textures;
        std::vector<size_t> indices;
        std::vector<glm::vec3> colors; // Store colors for each character batch

        for (int i = 0; i < text.size(); i++)
        {
            float xPos = x; // Reset x position for each line
            std::vector<Token> tokens = tokenize(text[i]);
            for (auto &token : tokens)
            {
                glm::vec3 color;
                switch (token.type)
                {
                case KEYWORD:
                    color = glm::vec3(0.0f, 1.0f, 0.0f); // Green
                    break;
                case IDENTIFIER:
                    color = glm::vec3(1.0f, 1.0f, 1.0f); // White
                    break;
                case NUMBER:
                    color = glm::vec3(1.0f, 1.0f, 0.0f); // Yellow
                    break;
                case STRING:
                    color = glm::vec3(1.0f, 0.0f, 1.0f); // Magenta
                    break;
                case OPERATOR:
                    color = glm::vec3(0.0f, 0.0f, 1.0f); // Blue
                    break;
                case COMMENT:
                    color = glm::vec3(0.5f, 0.5f, 0.5f); // Gray
                    break;
                default:
                    color = glm::vec3(1.0f, 1.0f, 1.0f); // Default to white
                    break;
                }

                for (auto c = token.text.begin(); c != token.text.end(); c++)
                {
                    Character ch = Characters[*c];

                    float xpos = xPos + ch.Bearing.x * scale;
                    float ypos = y - (ch.Size.y - ch.Bearing.y) * scale - (i * FONT_SIZE * scale); // Adjust y position

                    float w = ch.Size.x * scale;
                    float h = ch.Size.y * scale;

                    vertices.insert(vertices.end(), {xpos, ypos + h, 0.0f, 0.0f,
                                                     xpos, ypos, 0.0f, 1.0f,
                                                     xpos + w, ypos, 1.0f, 1.0f,
                                                     xpos, ypos + h, 0.0f, 0.0f,
                                                     xpos + w, ypos, 1.0f, 1.0f,
                                                     xpos + w, ypos + h, 1.0f, 0.0f});

                    textures.push_back(ch.TextureID);
                    indices.push_back(vertices.size() / 4 - 6); // Index of the first vertex for this character
                    colors.push_back(color);

                    xPos += (ch.Advance >> 6) * scale; // Advance cursor to next glyph
                }
            }
        }
        td.colors = colors;
        td.indices = indices;
        td.textures = textures;
        td.vertices = vertices;
    }

    // Bind the VBO and upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, td.vertices.size() * sizeof(float), td.vertices.data(), GL_DYNAMIC_DRAW);

    GLenum err;

    // Draw the characters
    for (size_t i = 0; i < td.textures.size(); i++)
    {
        glBindTexture(GL_TEXTURE_2D, td.textures[i]);

        // Set the text color for the current batch
        glUniform3f(glGetUniformLocation(s, "textColor"), td.colors[i].x, td.colors[i].y, td.colors[i].z);

        glDrawArrays(GL_TRIANGLES, td.indices[i], 6);
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void renderLineNumbers(GLuint &s, float x, float y, glm::vec3 color)
{
    glUseProgram(s);

    // Set the text color uniform
    GLint textColorLocation = glGetUniformLocation(s, "textColor");
    if (textColorLocation == -1)
    {
        std::cerr << "Failed to find uniform location for 'textColor'" << std::endl;
        return;
    }
    glUniform3f(textColorLocation, color.x, color.y, color.z);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    float scale = 1.0f; // Adjust scale as necessary
    if (line_nums_changed)
    {
        std::string converted;
        std::vector<float> vertices;
        std::vector<GLuint> textures;
        std::vector<size_t> indices;

        for (int i = 1; i <= e.lines.size(); i++)
        {
            converted = std::to_string(i);
            float xPos = x; // Start position for line number

            for (char c : converted)
            {
                Character ch = Characters[c];

                if (ch.TextureID == 0)
                {
                    std::cerr << "Invalid texture ID for character: " << c << std::endl;
                    continue;
                }

                float xpos = xPos + ch.Bearing.x * scale;
                float ypos = y - (ch.Size.y - ch.Bearing.y) * scale - ((i - 1) * FONT_SIZE * scale); // Adjust y position

                float w = ch.Size.x * scale;
                float h = ch.Size.y * scale;

                vertices.insert(vertices.end(), {xpos, ypos + h, 0.0f, 0.0f,
                                                 xpos, ypos, 0.0f, 1.0f,
                                                 xpos + w, ypos, 1.0f, 1.0f,
                                                 xpos, ypos + h, 0.0f, 0.0f,
                                                 xpos + w, ypos, 1.0f, 1.0f,
                                                 xpos + w, ypos + h, 1.0f, 0.0f});

                textures.push_back(ch.TextureID);
                indices.push_back(vertices.size() / 4 - 6); // Index of the first vertex for this character

                xPos += (ch.Advance >> 6) * scale; // Advance cursor to next glyph
            }
        }
        lnd.converted = converted;
        lnd.vertices = vertices;
        lnd.textures = textures;
        lnd.indices = indices;
    }

    // Bind the VBO and upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, lnd.vertices.size() * sizeof(float), lnd.vertices.data(), GL_DYNAMIC_DRAW);

    // Draw the characters
    for (size_t i = 0; i < lnd.textures.size(); i++)
    {
        glBindTexture(GL_TEXTURE_2D, lnd.textures[i]);
        glDrawArrays(GL_TRIANGLES, lnd.indices[i], 6);
    }

    // Unbind the VAO and VBO
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

glm::vec2 currentCameraPos(0.0f, 0.0f);
glm::vec2 targetCameraPos(0.0f, 0.0f);
float interpolationFactor = 0.3f; // Adjust for smoothness

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