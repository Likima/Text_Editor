#ifndef INPUT_PROCESSING_HPP
#define INPUT_PROCESSING_HPP

#include <glm/glm.hpp>

void talk();

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

#endif // INPUT_PROCESSING_HPP