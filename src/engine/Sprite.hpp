#pragma once

// #include <string>
// #include <fstream>
// #include <sstream>
// #include <iostream>

// #include "glm/mat2x2.hpp"
// #include "glm/mat3x3.hpp"
// #include "glm/mat4x4.hpp"
#include "../app.hpp"

class Sprite
{
public:
  Sprite();

  void scale(float s);
  void scale(float sx, float sy);

  void move(float tx, float ty);

  glm::mat4 getModelMtx();

  void draw(Shader *shader);

private:
  const int numberOfVertices = 4;

  GLuint VAO;
  GLuint VBO;
  GLuint texture1;

  glm::mat4 model = glm::mat4(1.0f);

  float *vertices;
  //----------------------------------------------------------------------------
  void setup() {
    glGenVertexArrays(1, &(this->VAO));
    glGenBuffers(1, &(this->VBO));
  }

  void bindVAO() {
    GLuint VAO = this->VAO;
    glBindVertexArray(VAO);
  }

  void upload() {
    GLuint VBO = this->VBO;

    const int positionLength = COMPONENTS_PER_VERTEX;
    const int texCoordLength = COMPONENTS_PER_VERTEX_TEXTURE;
    const int stride = positionLength + texCoordLength;
    int nVertices = this->numberOfVertices;
    size_t size = sizeof(float) * nVertices * positionLength * texCoordLength;
    float* vertices = this->vertices;

    this->bindVAO();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, positionLength, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*) (0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, texCoordLength, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
  }

  void loadTexture() {
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    // unsigned char *image = stbi_load("../texturas/awesomeface.png", &width, &height, &nrChannels, 0);
    // unsigned char *image = stbi_load("../texturas/sprites-cat-running.png", &width, &height, &nrChannels, 0);
    // unsigned char *image = stbi_load("../texturas/sprite.png", &width, &height, &nrChannels, 0);
    unsigned char *image = stbi_load("../texturas/goku.png", &width, &height, &nrChannels, 0);

    if(!image) {
        std::cout << "Falha ao carregar textura" << std::endl;
    }

    glGenTextures(1, &(this->texture1));
    glBindTexture(GL_TEXTURE_2D, this->texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);

    glBindTexture(GL_TEXTURE_2D, 0);
  }
};
