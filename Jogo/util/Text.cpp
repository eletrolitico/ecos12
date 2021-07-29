#include "Text.h"

#include <iostream>

#define CHAR_WIDTH 1.0f

Text *Text::instance = nullptr;

Text::Text() : Sprite("res/textures/font.png", CHAR_WIDTH, CHAR_WIDTH) {}

Text *Text::GetText()
{
    if (instance == nullptr)
        instance = new Text();

    return instance;
}

void Text::DrawString(Renderer r, glm::vec2 pos, float scale, const std::string &text)
{
    const char *cStr = text.c_str();
    while (*cStr != '\0')
    {
        int index = (int)*cStr;
        int x = index % 16;
        int y = index / 16;
        DrawPartial(r, pos, x * 8, m_Texture->GetHeight() - ((y + 1) * 8), (x + 1) * 8, m_Texture->GetHeight() - (y * 8), false, scale);

        pos.x += CHAR_WIDTH * scale;
        cStr++;
    }
}