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
    Game();
    ~Game();

    void start();
    void setup();
    void quit();

    void add(Sprite *sprite);
    void remove(Sprite *sprite);

    void setShader(Shader* shader);
    void setWorldMatrix(glm::mat4 worldMatrix);

private:
    GameState state;

    float width;
    float height;
    bool fullscreen;

    void init();
    void setupGlfw();
    void setupGlad();

    void renderSprites(std::vector<Sprite*> sprites, float currentTime);

    void processInput(GLFWwindow* window);
    void windowResized();

    Shader* shader;
    glm::mat4 worldMatrix;

    GLFWwindow *window;

    std::vector<Sprite*> sprites;
};