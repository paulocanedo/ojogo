#include "Texture.hpp"

Texture::Texture() {
  this->setup();
}

Texture::~Texture() {
  std::cout << __FUNCTION__ << ": " << this->id << std::endl;

  // glDeleteTextures(1, &this->id);
}

void Texture::setup() {
  glGenTextures(1, &this->id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::active() {
  glActiveTexture(GL_TEXTURE0);
}

void Texture::bind() {
  glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture::load(const char *file) {
  stbi_set_flip_vertically_on_load(true);
  unsigned char *image = stbi_load(file, &this->width, &this->height, &this->nrChannels, 0);
  if(!image) {
      std::cout << "Falha ao carregar textura" << std::endl;
  }

  this->bind();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
            this->nrChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  // stbi_image_free(image);

  glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::getWidth() {
  return this->width;
}

int Texture::getHeight() {
  return this->height;
}

int Texture::getNrChannels() {
  return this->nrChannels;
}

float Texture::getAspectRatio() {
  return this->getWidth() / (float)this->getHeight();
}
