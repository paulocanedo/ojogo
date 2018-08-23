#pragma once

#include "Animation.hpp"
#include "../app.hpp"
#include "Texture.hpp"

class Animation;

class Sprite
{
public:
  Sprite();
  virtual ~Sprite();

  void scale(float s);
  void scale(float sx, float sy);
  void translate(float tx, float ty);
  void rotate(float angle);

  void setModelMatrix(glm::mat4 mat);
  void setTexture(Texture *texture, int rows = 1, int columns = 1);
  void setCurrentElement(int row, int column);
  void nextFrame();

  glm::vec3 getTranslateVec();
  glm::vec3 getScaleVec();
  float getRotation();

  virtual void customSetup()  { std::cout << "Sprite::customSetup" << std::endl; }
  void setup();
  void update(float currentTime);
  void draw(Shader *shader);
  void gc();

  std::vector<Animation*> animations;

private:
  const int numberOfVertices = 4;

  GLuint vaoId;
  GLuint vboId;

  glm::vec3 mTranslate;
  glm::vec3 mScale;
  float angle;

  bool initialized = false;
  int currentFrame = 0;
  int rows;
  int columns;
  Texture *texture = nullptr;
  Animation* currentAnimation = nullptr;

  glm::mat4 model = glm::mat4(1.0f);
  //----------------------------------------------------------------------------
  void bindVAO();
  void upload();

  bool canDraw();

  glm::mat4 calculateModelMatrix(float tx, float ty, float sx, float sy, float angle);

};