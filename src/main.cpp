#include <iostream>
#include <string>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sprite.h"

using namespace std;

// Prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
int setupShader();
int setupSprite();
int loadTexture(const std::string& filePath);

const GLuint WIDTH = 800, HEIGHT = 800;

// Vertex and Fragment Shaders (com uniforms model e projection)
const GLchar* vertexShaderSource = R"(
#version 400
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texc;
out vec2 tex_coord;
uniform mat4 model;
uniform mat4 projection;
uniform vec2 offset;
uniform vec2 scale;
void main()
{
    tex_coord = offset + vec2(texc.x, 1.0 - texc.y) * scale;
    gl_Position = projection * model * vec4(position, 1.0);
}
)";
const GLchar* fragmentShaderSource = R"(
#version 400
in vec2 tex_coord;
out vec4 color;
uniform sampler2D tex_buff;
void main()
{
    color = texture(tex_buff, tex_coord);
}
)";

int main()
{
    // Inicialização GLFW
    if (!glfwInit())
    {
        std::cerr << "Erro ao inicializar GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Procesamento Grafico Atividade", NULL, NULL);
    if (!window)
    {
        std::cerr << "Erro ao criar janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // Inicializa GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Erro ao inicializar GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    // Setup shaders, sprite VAO e textura
    GLuint shader = setupShader();
    GLuint vao = setupSprite();
    GLuint tex = loadTexture("../assets/sprites/Pink_Monster_6.png");

    GLuint bgTex = loadTexture("../assets/background.png");
    GLuint bgVao = setupSprite();

    // Sprite
    Sprite pinkMonster(vao, tex, shader, glm::vec3(0,0,0), glm::vec3(400,300,1), 0.0, 6, 1, 12.0f);

    // Matriz de projeção ortográfica (viewport 800x800)
    glm::mat4 projection = glm::ortho(0.0f, float(WIDTH), 0.0f, float(HEIGHT), -1.0f, 1.0f);

    // Ativa blending para alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Loop principal
    float lastTime = glfwGetTime(); 
    while (!glfwWindowShouldClose(window))
    {
        float now = glfwGetTime();
        float deltaTime = now - lastTime;
        lastTime = now;
        
        glfwPollEvents();

        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        //Desenhe fundo
        glm::mat4 bgModel = glm::mat4(1.0f);
        bgModel = glm::translate(bgModel, glm::vec3(WIDTH/2, HEIGHT/2, 0.0f));
        bgModel = glm::scale(bgModel, glm::vec3(WIDTH, HEIGHT, 1.0f));

        glUseProgram(shader);
        glBindVertexArray(bgVao);
        glBindTexture(GL_TEXTURE_2D, bgTex);
        glUniform2f(glGetUniformLocation(shader, "offset"), 0.0f, 0.0f);
        glUniform2f(glGetUniformLocation(shader, "scale"), 1.0f, 1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(bgModel));
        glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
        /* glBindTexture(GL_TEXTURE_2D, 0); */

        //Controle de direção por teclado (WASD e setas)
        int direction = 0; // 0 = baixo, 1 = esquerda, 2 = direita, 3 = cima

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            direction = 3;
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            direction = 0;
        else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            direction = 1;
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            direction = 2;

        // Movimento do sprite
        float speed = 200.0f; // pixels por segundo
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            pinkMonster.move(0.0f, speed * deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            pinkMonster.move(0.0f, -speed * deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            pinkMonster.move(-speed * deltaTime, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            pinkMonster.move(speed * deltaTime, 0.0f);
        
        // ----- Limite nas bordas
        float halfWidth = 400.0f / 2.0f;
        float halfHeight = 300.0f / 2.0f;

        // Limite posição X
        if (pinkMonster.position.x < halfWidth)
            pinkMonster.position.x = halfWidth;
        if (pinkMonster.position.x > WIDTH - halfWidth)
            pinkMonster.position.x = WIDTH - halfWidth;

        // Limite posição Y
        if (pinkMonster.position.y < halfHeight)
            pinkMonster.position.y = halfHeight;
        if (pinkMonster.position.y > HEIGHT - halfHeight)
            pinkMonster.position.y = HEIGHT - halfHeight;
        
        // Atualiza e desenha o sprite animado conforme a direção
        pinkMonster.update(deltaTime, direction);
        pinkMonster.draw(projection);




        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void)scancode; // Silencia aviso
    (void)mods;     // Silencia aviso
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// ----------- Funções auxiliares -----------

int setupShader()
{
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "tex_buff"), 0);

    return shaderProgram;
}

int setupSprite()
{
    GLfloat vertices[] = {
        // x    y    z    s    t
        -0.5,  0.5, 0.0, 0.0, 1.0,
        -0.5, -0.5, 0.0, 0.0, 0.0,
         0.5,  0.5, 0.0, 1.0, 1.0,
         0.5, -0.5, 0.0, 1.0, 0.0
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Posição
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Texcoord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

int loadTexture(const std::string& filePath)
{
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format = GL_RGB;
        if (nrChannels == 4)
            format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texID;
}