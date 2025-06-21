#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sprite {
public:
    GLuint VAO;
    GLuint texture;
    GLuint shader;
    glm::vec3 position;
    glm::vec3 scale;
    float rotation;

    int framesX;
    int framesY;
    int currentFrame;
    int currentRow;
    float animTime;
    float frameDuration;

    Sprite(GLuint vao, GLuint tex, GLuint sh, glm::vec3 pos, glm::vec3 scale, float rotation,
           int framesX = 4, int framesY = 4, float fps = 8.0f);

    void update(float deltaTime, int direction);
    void move(float dx, float dy);
    void draw(const glm::mat4& projection);
};