#pragma once

#include "../app.hpp"

class Texture
{
public:
  Texture();
  ~Texture();

  void bind();
  void active();
  void load(const char *file);

  int getWidth();
  int getHeight();
  int getNrChannels();
  float getAspectRatio();

private:
  GLuint id = 0;
  int width;
  int height;
  int nrChannels;

  bool initialized = false;

  void setup();
};
