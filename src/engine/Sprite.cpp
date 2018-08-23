#include "Sprite.hpp"

Sprite::Sprite()
{
}

Sprite::~Sprite() {

}

void Sprite::scale(float s) {
  this->scale(s, s);
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

glm::vec3 Sprite::getTranslateVec() {
  return this->mTranslate;
}

glm::vec3 Sprite::getScaleVec() {
  return this->mScale;
}

float Sprite::getRotation() {
  return this->angle;
}

void Sprite::setTexture(Texture *texture, int rows, int columns) {
  this->texture = texture;
  this->rows = rows;
  this->columns = columns;
}

void Sprite::setCurrentElement(int currentRow, int currentColumn) {
  float rows = this->rows;
  float columns = this->columns;

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
  if(!this->initialized) {
    this->setup();
  }

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

  glDrawArrays(GL_TRIANGLE_STRIP, 0, nVertices);
}

void Sprite::gc() {
  if((this->vboId) > 0)
    glDeleteBuffers(1, &this->vboId);

  if((this->vaoId) > 0)
    glDeleteVertexArrays(1, &this->vaoId);
}

//--------------------------------------------
void Sprite::setup()
{
  glGenVertexArrays(1, &(this->vaoId));
  glGenBuffers(1, &(this->vboId));

  this->upload();
  this->initialized = true;

  if(this->rows > 1 || this->columns > 1) {
    this->nextFrame();
  }

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