#pragma once

#include "../app.hpp"
#include "Sprite.hpp"

class Sprite;

/**
 * A class representing abstract animation
 */
class Animation
{
public:
    /**
     * Constructor
     */
    Animation() {}

    /**
     * Destructor
     */
    virtual ~Animation();

    /**
     * An auxiliar method to fill some variables passing into updateFrame
     * @see updateFrame()
     * @return animation should continue
     */
    bool update(Sprite* sprite, float currentTime);

    /**
     * The abstract method to customize animation. Here you must describe how your sprite will animate.
     * 
     * @param sprite what the animation is attached
     * @param currentTime the current time cautch inside game loop. 
     *                    This one is provided from glfw library and means the seconds since the glfw was initialized.
     * @param ellapsedTime ellapsed time since last frame render
     * @param ellapsedTimeTotal ellapsed time since animation started
     * @param startLocation the location when animation started
     * 
     * @return animation should continue
     */
    virtual bool updateFrame(Sprite* sprite, float currentTime, float ellapsedTime, float ellapsedTimeTotal, glm::vec3 startLocation) = 0;

    // virtual float progress() = 0;

private:
    float startTime = -1.0f; /* after the first frame update, this will be filled */
    float lastFrameTime = -1.0f; /* control the time spent between frames updated */
    
    glm::vec3 startLocation; /* stores the initial location */
};