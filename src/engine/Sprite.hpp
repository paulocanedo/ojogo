#ifndef __SPRITE__
#define __SPRITE__

#include "../app.hpp"
#include "Texture.hpp"
#include "Animation.hpp"

class Sprite
{
public:
  //TODO adicionar opcao para informar as coordenadas iniciais do sprite
  Sprite(int rows = 1, int columns = 1, Texture *texture = nullptr);

  void scale(float s);
  void scale(float sx, float sy);
  void translate(float tx, float ty);
  void rotate(float angle);

  void setModelMatrix(glm::mat4 mat);
  void setTexture(Texture *texture);
  void setCurrentElement(int row, int column);
  void nextFrame();

  glm::vec3 getTranslateVec();
  glm::vec3 getScaleVec();
  float getRotation();

  void update(float currentTime);
  void draw(Shader *shader);
  void gc();

  Animation *animation = nullptr;

private:
  const int numberOfVertices = 4;

  GLuint vaoId;
  GLuint vboId;

  glm::vec3 mTranslate;
  glm::vec3 mScale;
  float angle;

  int currentFrame = 0;
  int rows;
  int columns;
  Texture *texture;
  std::vector<Animation> animations;

  glm::mat4 model = glm::mat4(1.0f);
  //----------------------------------------------------------------------------
  void setup();
  void bindVAO();
  void upload();

  void recalculateModelMatrix();

};

#endif