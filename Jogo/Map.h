#ifndef MAP_H
#define MAP_H

#include <string>
#include <memory>
#include <vector>
#include <set>
#include <unordered_map>
#include <glm/glm.hpp>

#include "Renderer.h"
#include "Tile.h"
#include "VertexBuffer.h"
#include "Texture.h"

class Map
{
public:
    Map(std::string map, int w, int h, int, int, float);
    ~Map();
    void draw(Renderer r, glm::mat4 mvp, glm::vec2 li) const;
    void update(float fElapsedTime);
    char getMap(int x, int y) const;
    bool getCollide(float x1, float y1, float x2, float y2) const;
    bool getDanger(float x1, float y1, float x2, float y2) const;
    Tile getTile(char c, int width) const;
    glm::vec3 getInitialPos() const;
    void addInvInterval(glm::vec2 interval);
    bool isInverted(glm::vec2 pos) const;

    int m_width, m_height;
    std::unique_ptr<Shader> m_Shader;

private:
    std::string m_Map;

    bool m_Transparent[256];

    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<Texture> m_Texture;
    std::vector<Tile> m_Tiles;

    float m_AmbientLight;

    glm::vec3 m_InitialPos;

    std::vector<glm::vec2> m_InvInterval;
};

#endif