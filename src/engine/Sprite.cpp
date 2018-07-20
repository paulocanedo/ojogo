#include "Sprite.hpp"

Sprite::Sprite(int rows, int columns, Texture *texture) {
  this->rows = rows;
  this->columns = columns;

  this->setup();
  this->upload();
  this->setTexture(texture);
}

void Sprite::scale(float s) {
  this->scale(s, s);
}

void Sprite::scale(float sx, float sy) {
  this->model = glm::scale(this->model, glm::vec3(sx, sy, 1.0f));
}

void Sprite::move(float tx, float ty) {
  this->model = glm::translate(this->model, glm::vec3(tx, ty, 0.0f));
}

glm::mat4 Sprite::getModelMtx() {
  return this->model;
}

void Sprite::setTexture(Texture *texture) {
  this->texture = texture;
}

void Sprite::draw(Shader *shader = nullptr) {
  int nVertices = this->numberOfVertices;

  this->bindVAO();
  this->texture->active();
  this->texture->bind();

  if(shader != nullptr) {
    shader->use();

    shader->setMat4("model", this->getModelMtx());
    shader->setInt("ourTexture1", 0);
  }

  glDrawArrays(GL_TRIANGLE_STRIP, 0, nVertices);
}

void Sprite::setCurrentElement(int row, int column) {
  float rows = this->rows;
  float columns = this->columns;

  float texCoords[] = {
    column / columns,       (row + 1) / rows, //left top
    column / columns,       row / rows,       //left bottom
    (column + 1) / columns, (row + 1) / rows, //right top
    (column + 1) / columns, row / rows,       //right bottom
  };

  int nVertices = this->numberOfVertices;
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * nVertices * COMPONENTS_PER_VERTEX, sizeof(float) * nVertices * COMPONENTS_PER_VERTEX_TEXTURE, &texCoords);
}

void Sprite::gc() {
  if((this->vboId) > 0)
    glDeleteBuffers(1, &this->vboId);

  if((this->vaoId) > 0)
    glDeleteVertexArrays(1, &this->vaoId);
}
