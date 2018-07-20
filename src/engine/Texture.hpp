#pragma once

#include "../app.hpp"

class Texture
{
public:
  Texture();

  void bind();
  void active();
  void load(const char *file);
  void free();

  int getWidth();
  int getHeight();
  int getNrChannels();

private:
  GLuint id;
  int width;
  int height;
  int nrChannels;

  void setup();
};
