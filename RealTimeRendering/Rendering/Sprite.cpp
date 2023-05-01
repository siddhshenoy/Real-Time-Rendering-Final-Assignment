#include "Sprite.h"

std::vector<Sprite*> Sprite::gSpriteList;
int Sprite::gSpriteCounter = 0;

Sprite::Sprite()
{
    mColor = glm::vec4(1.0f);
    this->IsVisible = true;
    this->SetName(("Sprite_" + std::to_string(Sprite::gSpriteCounter)).c_str());
    Sprite::gSpriteCounter++;
    Sprite::gSpriteList.push_back(this);
    //spdlog::info("Sprite List size: " + std::to_string(Sprite::gSpriteList.size()));
}
Sprite::Sprite(const std::string& sprite_image_path)
{
    this->LoadSpriteFromPath(sprite_image_path);
    mColor = glm::vec4(1.0f);
    this->IsVisible = true;
    this->SetName(("Sprite_" + std::to_string(Sprite::gSpriteCounter)).c_str());
    Sprite::gSpriteCounter++;
    Sprite::gSpriteList.push_back(this);
    //spdlog::info("Sprite List size: " + std::to_string(Sprite::gSpriteList.size()));
}

void Sprite::SetColor(const glm::vec4& color)
{
    mColor = color;
}
void Sprite::LoadSpriteFromPath(const std::string& sprite_image_path) {
    spdlog::info("Loading sprite from path: {}", sprite_image_path.c_str());
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(2, mVBO);
    texture = new Texture();
    texture->LoadTexture(sprite_image_path.c_str());
    /*float vertices[] = {
         1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 1.0f
    };*/
    float vertices[] = {
     1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,
     1.0f,  1.0f, 1.0f, 1.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f, 1.0f
    };
    //float vertices[] = {
    //    // Positions       // Texture coords
    //    -1.0f, -1.0f, 1.0f, 1.0f,
    //     1.0f, -1.0f, 0.0f, 1.0f,
    //     1.0f,  1.0f, 0.0f, 0.0f,
    //    -1.0f,  1.0f, 1.0f, 0.0f
    //};

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
glm::vec4 Sprite::GetColor()
{
    return mColor;
}
void Sprite::SetVisible(bool set)
{
    this->IsVisible = set;
}
void Sprite::AssignShader(Shader* shader) {
    TextShader = shader;
}
void Sprite::Render(Camera* cam)
{
    if (this->IsVisible) {
        glm::mat4 final_matrix = cam->GetProjectionMatrix() * this->GetTransform();
        this->TextShader->UseShader();
        this->TextShader->SetUniformMatrix("transform", 1, GL_FALSE, &final_matrix[0][0]);
        this->TextShader->SetUniform4f("sprite_color", mColor[0], mColor[1], mColor[2], mColor[3]);
        texture->BindTexture();
        glBindVertexArray(mVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

}
void Sprite::RenderBlank()
{
    this->TextShader->UseShader();
    //texture->BindTexture();
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}
void Sprite::RemoveSpriteFromList(Sprite* sprite) {
    std::vector<Sprite*>::iterator loc = std::find(Sprite::gSpriteList.begin(), Sprite::gSpriteList.end(), sprite);
    if (loc != Sprite::gSpriteList.end()) {
        if (*loc != nullptr)
            Sprite::gSpriteList.erase(loc);
    }
}
Sprite::~Sprite()
{    
    Sprite::RemoveSpriteFromList(this);
    glDeleteBuffers(2, mVBO);
    glDeleteVertexArrays(1, &mVAO);
}
