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

  /**
   * set sprite scale
   * @param sx scale of X
   * @param sy scale of Y
   */
  void scale(float sx, float sy);

  /**
   * set sprite position
   * @param tx position of X
   * @param ty position of Y
   */
  void translate(float tx, float ty);

  /**
   * set sprite rotation based on axis X: 0, Y: 0, Z: 1
   * @param angle rotation value in radians
   */
  void rotate(float angle);

  /**
   * set model matrix (MVP) transformation. (https://learnopengl.com/Getting-started/Coordinate-Systems)
   * @param mat the mvp matrix transformation
   */
  void setModelMatrix(glm::mat4 mat);

  /**
   * set current image. use this method for multi image texture based
   * @param row index of the image, zero-based
   * @param column index of the image, zero-based
   * @see nextFrame()
   */
  void setCurrentElement(int row, int column);

  /**
   * call setCurrentElement() using the next index, increases row-index and then column index
   * @see setCurrentElement()
   */
  void nextFrame();

  /**
   * get sprite width
   * @return sprite width
   */
  float getWidth();

  /**
   * get sprite height
   * @return sprite height
   */
  float getHeight();

  /**
   * get the current vector representing sprite position
   * @return translate vector
   */
  glm::vec3 getTranslateVec();

  /**
   * get the current vector representing sprite size
   * @return scale vector
   */
  glm::vec3 getScaleVec();

  /**
   * get the rotation angle, in radians, of the sprite
   * @return rotation angle, in radians
   */
  float getRotation();

  /**
   * an abstract method to be called after setup method is called
   */
  virtual void customSetup() {};

  /**
   * Initializes the sprite, this method is called after first update call,
   * this strategy allows developer to get a new instance of sprite before 
   * graphics library is initialized, once this method require some graphics 
   * function calls.
   */
  void setup();

  /**
   * updates sprite state based on animation objects
   * @param currentTime time when the current iterator game loop frame starts
   */
  void update(float currentTime);

  /**
   * draws the sprite
   */
  void draw(Shader *shader);

  /**
   * free resources from graphics library
   */
  void gc();

  /**
   * List of animations
   */
  std::vector<Animation*> animations;

private:
  const int numberOfVertices = 4;

  GLuint vaoId = 0;
  GLuint vboId = 0;

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