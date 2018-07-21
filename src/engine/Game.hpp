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

    void start();
    void quit();

    void add(Sprite *sprite);
    void remove(Sprite *sprite);

private:
    GameState state;

    float width;
    float height;
    bool fullscreen;

    void setup();
    void setupGlfw();
    void setupGlad();

    void windowResized();

};