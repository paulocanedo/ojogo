#pragma once

#include "../app.hpp"
#include "Texture.hpp"

class Sprite
{
public:
  Sprite(int rows, int columns, Texture *texture);

  void scale(float s);
  void scale(float sx, float sy);

  void move(float tx, float ty);

  glm::mat4 getModelMtx();

  void setTexture(Texture *texture);
  void draw(Shader *shader);
  void setCurrentElement(int row, int column);
  void gc();

private:
  const int numberOfVertices = 4;

  GLuint vaoId;
  GLuint vboId;
  GLuint texture1;

  int rows;
  int columns;
  Texture *texture;

  glm::mat4 model = glm::mat4(1.0f);
  //----------------------------------------------------------------------------
  void setup() {
    glGenVertexArrays(1, &(this->vaoId));
    glGenBuffers(1, &(this->vboId));
  }

  void bindVAO() {
    GLuint vaoId = this->vaoId;
    glBindVertexArray(vaoId);
  }

  void upload() {
    float vertices[] = {
      0.0f, 1.0f,
      0.0f, 0.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,

      0.0f, 1.0f,
      0.0f, 0.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
    };

    const int positionLength = COMPONENTS_PER_VERTEX;
    const int texCoordLength = COMPONENTS_PER_VERTEX_TEXTURE;

    int nVertices = this->numberOfVertices;

    this->bindVAO();
    glBindBuffer(GL_ARRAY_BUFFER, this->vboId);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nVertices * positionLength * texCoordLength, &vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, positionLength, GL_FLOAT, GL_FALSE, positionLength * sizeof(GLfloat), (GLvoid*) (0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, texCoordLength, GL_FLOAT, GL_FALSE, texCoordLength * sizeof(GLfloat), (GLvoid*) (numberOfVertices * positionLength * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
  }

};
