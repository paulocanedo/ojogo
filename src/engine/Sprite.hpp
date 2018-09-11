#pragma once

#include "../app.hpp"

#include "Animation.hpp"
#include "Texture.hpp"

class Animation;

/**
 * A class representing a game sprite. Currently only texture draw/fill is supported
 */
class Sprite
{
public:
  /**
  * Constructor
  */
  Sprite();

  /**
  * Destructor
  */
  virtual ~Sprite();

  /**
  * Alloc a sprite with a simple texture based
  * @param filename path to image
  * @param width dimension of the sprite, height will be calculate respecting the aspect of texture.
  *              If you wanna to define height, please use scale() method
  * @see scale()
  * @see fromMultiImage
  * @return A sprite with texture attached
  */
  static std::shared_ptr<Sprite> fromTexture(const char* filename, float width = 10.0f);

  /**
  * Alloc a sprite with a multi image texture based
  * @param filename path to image
  * @param width dimension of the sprite, height will be calculate respecting the aspect of texture.
  *              If you wanna to define height, please use scale() method
  * @param rows number of rows from multi image texture
  * @param columns number of columns from multi image texture
  * @see scale()
  * @see nextFrame()
  * @see setCurrentElement
  * @return A sprite with texture attached
  */
  static std::shared_ptr<Sprite> fromMultiImage(const char *filename, float width, int rows, int columns);

  /**
   * set sprite scale for both x and y
   * @param s scale value
   */
  void scale(float s);

  /**
   * set sprite scale based on x and texture aspect
   * @param sx scale of x, the y value will be calculate based on aspect
   */
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
  float angle = 0.0f;

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