#pragma once

#include "Sprite.hpp"

enum GameState {
    MENU,
    PLAYING,
    PAUSE,
    GAME_OVER
};

class Game
{
public:
  Game(unsigned short width = 800, unsigned short height = 600);
  ~Game();

  void start();
  void setup();
  void quit();
  void gc();

  void add(std::shared_ptr<Sprite> sprite);
  void remove(std::shared_ptr<Sprite> sprite);

  void setShader(Shader *shader);
  void setWorldMatrix(glm::mat4 worldMatrix);

private:
    GameState state;

    float width;
    float height;
    bool fullscreen;

    void init();
    void setupGlfw();
    void setupGlad();

    void renderSprites(std::vector<std::shared_ptr<Sprite>> sprites, float currentTime);

    void processInput(GLFWwindow* window);
    void windowResized();

    Shader* shader;
    glm::mat4 worldMatrix;

    GLFWwindow *window;

    std::vector<std::shared_ptr<Sprite>> sprites;
};