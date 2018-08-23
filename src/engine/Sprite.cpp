#include "Sprite.hpp"

Sprite::Sprite()
{
  this->setup();
}

Sprite::~Sprite() {
  if(initialized) {
    if ((this->vboId) > 0)
      glDeleteBuffers(1, &this->vboId);

    if ((this->vaoId) > 0)
      glDeleteVertexArrays(1, &this->vaoId);
  }

  std::cout << __FUNCTION__ << ": " << this->vboId << ";" << this->vaoId << std::endl;
}

std::shared_ptr<Sprite> Sprite::fromTexture(const char* filename, float width) {
  std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>();

  sprite->texture = std::make_unique<Texture>();
  sprite->texture->load(filename);
  sprite->scaleWithAspect(width);

  return sprite;
}

std::shared_ptr<Sprite> Sprite::fromMultiImage(const char *filename, float width, int rows, int columns) {
  std::shared_ptr<Sprite> sprite = Sprite::fromTexture(filename, width);

  sprite->rows = rows;
  sprite->columns = columns;
  sprite->setCurrentElement(0, 0);

  return sprite;
}

void Sprite::scale(float s) {
  this->scale(s, s);
}

void Sprite::scaleWithAspect(float sx) {
  float sy = sx * this->getHeight() / this->getWidth();
  this->scale(sx, sy);
}

void Sprite::scale(float sx, float sy) {
  this->mScale.x = sx;
  this->mScale.y = sy;

  this->model = this->calculateModelMatrix(this->mTranslate.x, this->mTranslate.y, sx, sy, this->angle);
}

void Sprite::translate(float tx, float ty) {
  this->mTranslate.x = tx;
  this->mTranslate.y = ty;

  this->model = this->calculateModelMatrix(tx, ty, this->mScale.x, this->mScale.y, this->angle);
}

void Sprite::rotate(float angle) {
  this->angle = angle;

  this->model = this->calculateModelMatrix(this->mTranslate.x, this->mTranslate.y,
                                           this->mScale.x, this->mScale.y,
                                           angle);
}

void Sprite::setModelMatrix(glm::mat4 mat) {
  this->model = mat;
}

float Sprite::getWidth() {
  if(texture) {
    return texture->getWidth();
  }

  std::cerr << __FUNCTION__ << ": not implemented yet for non-texture" << std::endl;
  return 0.0f;
}

float Sprite::getHeight() {
  if (texture) {
    return texture->getHeight();
  }

  std::cerr << __FUNCTION__ << ": not implemented yet for non-texture" << std::endl;
  return 0.0f;
}

glm::vec3 Sprite::getTranslateVec() {
  return this->mTranslate;
}

glm::vec3 Sprite::getScaleVec() {
  return this->mScale;
}

float Sprite::getRotation() {
  return this->angle;
}

void Sprite::setCurrentElement(int currentRow, int currentColumn) {
  float rows = this->rows;
  float columns = this->columns;

  if (rows < 1 && columns < 1) {
    std::cerr << __FUNCTION__ << ": should not call this function without define columns and rows" << std::endl;
  }

  currentRow = (this->rows - 1) - currentRow;

  float texCoords[] = {
    currentColumn / columns,       (currentRow + 1) / rows, //left top
    currentColumn / columns,       currentRow / rows,       //left bottom
    (currentColumn + 1) / columns, (currentRow + 1) / rows, //right top
    (currentColumn + 1) / columns, currentRow / rows,       //right bottom
  };

  // float *texCoords = this->getTexCoords(currentRow, currentColumn);

  int nVertices = this->numberOfVertices;
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * nVertices * COMPONENTS_PER_VERTEX, sizeof(float) * nVertices * COMPONENTS_PER_VERTEX_TEXTURE, &texCoords);
}

void Sprite::update(float currentTime) {
  // if(!this->initialized) {
  //   this->setup();
  // }

  if(this->currentAnimation != nullptr) {
    Animation *animation = this->currentAnimation;
    bool shouldContinue = animation->update(this, currentTime);

    if(!shouldContinue) {
      this->animations.erase(this->animations.begin());
      this->currentAnimation = nullptr;
    }
  } else {
    if (!this->animations.empty()) {
      this->currentAnimation = this->animations.front();
    }
  }
}

void Sprite::draw(Shader *shader = nullptr) {
  if(!this->canDraw()) return;

  int nVertices = this->numberOfVertices;

  this->bindVAO();
  this->texture->active();
  this->texture->bind();

  if(shader != nullptr) {
    shader->use();

    shader->setMat4("model", this->model);
    shader->setInt("ourTexture1", 0);
  }

  // if (shader != nullptr) {
  //   shader->use();

  //   glm::mat4 myModel(1.0f);

  //   myModel = glm::translate(myModel, glm::vec3(50.0f, 50.0f, 0.0f));
  //   myModel = glm::scale(myModel, glm::vec3(256.0f, 256.0f, 1.0f));

  //   shader->setMat4("model", myModel);
  //   shader->setInt("ourTexture1", 0);
  // }

  glDrawArrays(GL_TRIANGLE_STRIP, 0, nVertices);
}

//--------------------------------------------
void Sprite::setup()
{
  glGenVertexArrays(1, &(this->vaoId));
  glGenBuffers(1, &(this->vboId));

  this->upload();
  this->initialized = true;

  this->customSetup();
}

void Sprite::bindVAO()
{
  GLuint vaoId = this->vaoId;
  glBindVertexArray(vaoId);
}

void Sprite::upload()
{
  float vertices[] = {
      0.0f, 1.0f, //top left
      0.0f, 0.0f, //bottom left
      1.0f, 1.0f, //top right
      1.0f, 0.0f, //bottom right

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

  glVertexAttribPointer(0, positionLength, GL_FLOAT, GL_FALSE, positionLength * sizeof(GLfloat), (GLvoid *)(0 * sizeof(GLfloat)));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, texCoordLength, GL_FLOAT, GL_FALSE, texCoordLength * sizeof(GLfloat), (GLvoid *)(numberOfVertices * positionLength * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
}

glm::mat4 Sprite::calculateModelMatrix(float tx, float ty, float sx, float sy, float angle) {
  glm::mat4 mat(1.0);

  mat = glm::translate(mat, glm::vec3(tx, ty, 0.0f));
  mat = glm::scale(mat, glm::vec3(sx, sy, 1.0f));
  mat = glm::rotate(mat, angle, glm::vec3(0.0f, 0.0f, 1.0f));

  std::cout << "angle: " << angle << std::endl;

  return mat;
}

void Sprite::nextFrame() {
  int i=0, j=0;
  this->currentFrame++;
  if(this->currentFrame >= this->rows * this->columns) {
    this->currentFrame = 0;
  } else {
    if (this->currentFrame >= this->columns)
    {
      i = (int)(this->currentFrame / this->columns);
    }
    j = this->currentFrame % this->columns;
  }

  this->setCurrentElement(i, j);
}

bool Sprite::canDraw() {
  return (this->vaoId >= 0 && this->vboId >= 0);
}