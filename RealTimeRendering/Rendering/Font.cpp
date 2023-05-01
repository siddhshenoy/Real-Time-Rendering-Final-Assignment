#include "Font.h"

Font::Font()
{
    Font::IsFTLoaded = false;
    this->mVAO = 0;
    this->mVBO = 0;
    this->FreeTypeLib = nullptr;
    this->mShader = nullptr;
	if (!IsFTLoaded) {
		std::cout << __FUNCTION__ << " FreeType font library was not initialized, trying to initialize it!" << std::endl;
		if (FT_Init_FreeType(&FreeTypeLib))
		{
			std::cout << __FUNCTION__ << " ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            return;
		}
		IsFTLoaded = true;

        glGenVertexArrays(1, &mVAO);
        glGenBuffers(1, &mVBO);
        glBindVertexArray(mVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
	}
}

void Font::AssignShader(Shader* shader)
{
    mShader = shader;
}

void Font::LoadFont(const std::string& font_path)
{
	if (IsFTLoaded) {
        FT_Face face;
        if (FT_New_Face(Font::FreeTypeLib, font_path.c_str(), 0, &face)) {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            return;
        }
        else {
            FT_Set_Pixel_Sizes(face, 0, 48);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            for (unsigned char c = 0; c < 128; c++)
            {
                // Load character glyph 
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                    continue;
                }
                // generate texture
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
                };
                mCharacters.insert(std::pair<char, Character>(c, character));
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        FT_Done_Face(face);
        FT_Done_FreeType(FreeTypeLib);
	}
	else {
		std::cerr
			<< __FUNCTION__
			<< " Trying to load font without the font library being pre-loaded."
			<< " There might be some errors while loading the font library, please check them.."
			<< std::endl;
	}
}

void Font::RenderText(float screenWidth, float screenHeight, const std::string& text, float x, float y, const float scale, const glm::vec3 color, glm::vec2* rPosition, glm::vec2* rDimensions)
{
    //Text2D* RenderedFont = new Text2D();
    //std::cout << "Rendering text" << std::endl;
    mShader->UseShader();
    glm::mat4 projectionMatrix = glm::ortho(0.0f, screenWidth, 0.0f, screenHeight);
    mShader->SetUniformMatrix(const_cast<GLchar*>("projection"), 1, GL_FALSE, &projectionMatrix[0][0]);
    mShader->SetUniform3f(const_cast<GLchar*>("TextColor"), color.x, color.y, color.z);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(mVAO);

    // iterate through all characters
    float minX = 10000.0f, minY = 10000.0f, maxX = 0.0f, maxY = 0.0f;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = mCharacters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
       
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        if ((xpos) < minX) minX = xpos;
        if ((ypos) < minY) minY = ypos;
        if ((xpos + w) > maxX) maxX = xpos + w;
        if ((ypos + h) > maxY) maxY = ypos + h;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    if (rPosition != nullptr) {
        rPosition->x = minX;
        rPosition->y = minY;
    }
    if (rDimensions != nullptr) {
        rDimensions->x = (maxX - minX);
        rDimensions->y = (maxY - minY);
    }
  
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Font::~Font()
{
}
