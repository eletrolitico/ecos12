#include "Map.h"

#include "VertexBufferLayout.h"

Map::~Map()
{
}

Map::Map(std::string map, int w, int h, int iniX, int iniY, float ambientLight) : m_width(w), m_height(h), m_Map(map), m_InitialPos({iniX, iniY, 0.0f}), m_AmbientLight(ambientLight)
{
    int fSize = m_height * m_width * 24;
    float *positions = new float[fSize];

    m_Texture = std::make_unique<Texture>("res/textures/Tilemap.png");
    m_Texture->Unbind();

    int texSize = 16;
    int tw = m_Texture->GetWidth(), th = m_Texture->GetHeight();

    for (int j = 0; j < th / texSize; ++j)
    {
        for (int i = 0; i < tw / texSize; ++i)
        {
            m_Tiles.push_back(Tile(i * texSize, th - j * texSize, tw, th, texSize));
        }
    }

    int index = 0;
    for (int i = 0; i < m_width; ++i)
    {
        for (int j = 0; j < m_height; ++j)
        {
            char mP = m_Map[(m_height - 1 - j) * m_width + i];
            if (mP != '.')
            {
                float *temp = getTile(mP, tw / texSize).draw(i, j);

                for (int k = 0; k < 24; ++k)
                {
                    positions[index++] = temp[k];
                }
                delete[] temp;
            }
        }
    }

    // Vertex array.
    m_VAO = std::make_unique<VertexArray>();

    // Vertex buffer
    m_VertexBuffer = std::make_unique<VertexBuffer>(positions, fSize);

    // Set attributes.
    VertexBufferLayout lay;
    lay.Push<float>(2);
    lay.Push<float>(2);

    m_VAO->AddBuffer(*m_VertexBuffer, lay);

    // Unbind Vertex Array Object.
    m_VAO->Unbind();

    m_Shader = std::make_unique<Shader>("res/shaders/Tile.shader");

    for (int i = 0; i < 256; ++i)
        m_Transparent[i] = false;
    m_Transparent['.'] = true;
    m_Transparent['W'] = true;
    m_Transparent['O'] = true;
    m_Transparent['P'] = true;

    delete[] positions;
}

Tile Map::getTile(char c, int width) const
{
    switch (c)
    {
    case 'R':
        return m_Tiles[1];
    case 'T':
        return m_Tiles[2];
    case 'Y':
        return m_Tiles[3];
    case 'E':
        return m_Tiles[width + 1];
    case 'G':
        return m_Tiles[width + 2];
    case 'D':
        return m_Tiles[width + 3];
    case 'L':
        return m_Tiles[2 * width + 1];
    case 'K':
        return m_Tiles[2 * width + 3];
    case 'U':
        return m_Tiles[2 * width + 2];
    case 'A':
        return m_Tiles[3 * width + 1];
    case 'S':
        return m_Tiles[3 * width + 2];
    case 'B':
        return m_Tiles[3 * width + 3];
    case 'O':
        return m_Tiles[4];
    case 'F':
        return m_Tiles[5];
    case 'H':
        return m_Tiles[6];
    case 'W':
        return m_Tiles[7];
    case 'P':
        return m_Tiles[8];
    }
    return m_Tiles[0];
}

void Map::draw(Renderer r, glm::mat4 mvp, glm::vec2 li) const
{
    m_Texture->Bind();
    m_Shader->Bind();
    m_Shader->setUniformMat4f("u_MVP", mvp);
    m_Shader->setUniform1f("u_AmbientLight", m_AmbientLight);
    m_Shader->setUniform2f("lightPos[0]", li.x, li.y);
    r.Draw(*m_VAO, *m_Shader);
}

char Map::getMap(int x, int y) const
{
    return x >= 0 && x < m_width && y >= 0 && y < m_height ? m_Map[(m_height - 1 - y) * m_width + x] : '_';
}

bool Map::getDanger(float x1, float y1, float x2, float y2) const
{
    const char dangers[] = {'O', 'P'};
    for (char d : dangers)
    {
        if (getMap(x1, y1) == d)
            return true;
        if (getMap(x1, y2) == d)
            return true;
        if (getMap(x2, y1) == d)
            return true;
        if (getMap(x2, y2) == d)
            return true;
    }
    return false;
}

glm::vec3 Map::getInitialPos() const
{
    return m_InitialPos;
}

bool Map::getCollide(float x1, float y1, float x2, float y2) const
{
    x1 = floor(x1);
    x2 = floor(x2);
    bool c1 = m_Transparent[getMap(x1, y1)];
    bool c4 = m_Transparent[getMap(x1, y2)];
    bool c3 = m_Transparent[getMap(x2, y1)];
    bool c2 = m_Transparent[getMap(x2, y2)];
    return !(c1 && c2 && c3 && c4);
}

void Map::addInvInterval(glm::vec2 interval)
{
    m_InvInterval.push_back(interval);
}

bool Map::isInverted(glm::vec2 pos) const
{
    for (glm::vec2 in : m_InvInterval)
    {
        if (pos.x > in.x && pos.x < in.y)
            return true;
    }
    return false;
}

void Map::update(float fElapsedTime)
{
}