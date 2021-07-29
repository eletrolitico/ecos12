#ifndef TEXT_H
#define TEXT_H

#include "Sprite.h"

class Text : private Sprite
{
public:
    Text(Text &other) = delete;
    void operator=(const Text &) = delete;
    static Text *GetText();

    void DrawString(Renderer r, glm::vec2 pos, float scale, const std::string &text);

private:
    Text();

    static Text *instance;
};

#endif