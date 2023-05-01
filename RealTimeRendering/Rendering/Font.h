/*
    Reference: https://learnopengl.com/In-Practice/Text-Rendering
*/
#pragma once


#include <iostream>
#include <map>
#include <string>

#include <Rendering/Camera.h>
#include <Rendering/Shader.h>
#include <Resource/ResourceManager.h>
#include <glew/GL/glew.h>
#include <glm/glm/glm.hpp>
#include <freetype/ft2build.h>
#include <freetype/freetype/freetype.h>

/// Holds all state information relevant to a character as loaded using FreeType
typedef struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
}
Character;

class Font
{
private:
    GLuint mVAO;
    GLuint mVBO;
	FT_Library FreeTypeLib;
    bool IsFTLoaded;
    std::map<GLchar, Character> mCharacters;
    Shader* mShader;
public:
    Font();
    void AssignShader(Shader* shader);
    void LoadFont(const std::string& font_path);
    void RenderText(float screenWidth, float screenHeight, const std::string& text, float x, float y, const float scale, const glm::vec3 color, glm::vec2* rPosition = nullptr, glm::vec2* rDimensions = nullptr);
    ~Font();
};

