#include "../app.hpp"

Sprite::Sprite() {
  float vertices[] = {
    //X   Y     S     T
    0.0f, 1.0f, 0.0f, 1.0f, //left top
    0.0f, 0.0f, 0.0f, 0.0f, //left bottom
    1.0f, 1.0f, 1.0f, 1.0f, //right top
    1.0f, 0.0f, 1.0f, 0.0f, //right bottom
  };

  this->vertices = &(*vertices);

  this->setup();
  this->upload();
  this->loadTexture();
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

void Sprite::draw(Shader *shader) {
  int nVertices = this->numberOfVertices;
  GLuint texture = this->texture1;

  if(shader != nullptr) {
    shader->use();
  }

  this->bindVAO();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  shader->setMat4("model", this->getModelMtx());
  shader->setInt("ourTexture1", 0);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, nVertices);
}
