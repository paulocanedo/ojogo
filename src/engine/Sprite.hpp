#pragma once

#include "Texture.hpp"

class Sprite
{
public:
  Sprite(int rows = 1, int columns = 1, Texture *texture = nullptr);

  void scale(float s);
  void scale(float sx, float sy);
  void translate(float tx, float ty);
  void rotate(float angle);

  void setModelMatrix(glm::mat4 mat);
  void setTexture(Texture *texture);
  void setCurrentElement(int row, int column);

  glm::vec3 getTranslateVec();
  glm::vec3 getScaleVec();
  float getRotation();

  void draw(Shader *shader);
  void gc();

private:
  const int numberOfVertices = 4;

  GLuint vaoId;
  GLuint vboId;

  glm::vec3 mTranslate;
  glm::vec3 mScale;
  float angle;

  int rows;
  int columns;
  Texture *texture;

  glm::mat4 model = glm::mat4(1.0f);
  //----------------------------------------------------------------------------
  void setup();
  void bindVAO();
  void upload();

  void recalculateModelMatrix();

};
