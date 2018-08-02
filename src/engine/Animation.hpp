#ifndef __ANIMATION__
#define __ANIMATION__

#include "../app.hpp"

class Animation
{
public:
    virtual bool update(float currentTime) = 0;
};

#endif