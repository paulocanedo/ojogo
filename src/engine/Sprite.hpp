#pragma once

#include "../app.hpp"

#include "Animation.hpp"
#include "Texture.hpp"

class Animation;

class Sprite
{
public:
  Sprite();
  virtual ~Sprite();

  static std::shared_ptr<Sprite> fromTexture(const char* filename, float width = 10.0f);
  static std::shared_ptr<Sprite> fromMultiImage(const char *filename, float width, int rows, int columns);

  void scale(float s);
  void scaleWithAspect(float sx);
  void scale(float sx, float sy);
  void translate(float tx, float ty);
  void rotate(float angle);

  void setModelMatrix(glm::mat4 mat);
  void setCurrentElement(int row, int column);
  void nextFrame();

  float getWidth();
  float getHeight();
  glm::vec3 getTranslateVec();
  glm::vec3 getScaleVec();
  float getRotation();

  virtual void customSetup() {};
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

  std::unique_ptr<Texture> texture;
  Animation* currentAnimation = nullptr;

  glm::mat4 model = glm::mat4(1.0f);
  //----------------------------------------------------------------------------
  void bindVAO();
  void upload();

  bool canDraw();

  glm::mat4 calculateModelMatrix(float tx, float ty, float sx, float sy, float angle);
};