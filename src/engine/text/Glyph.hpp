#pragma once

#include "../../app.hpp"

#define BEZIER_STEP 1 / 3.0f

class Glyph
{
private:
    const FT_Face &face;
    const FT_ULong &code;

    std::vector<std::vector<glm::vec2>> contours;

    void load(const FT_Face &_face, const FT_ULong &_code);

    static int moveToFunction(FT_Vector *to, void *user);
    static int lineToFunction(FT_Vector *to, void *user);
    static int conicToFunction(FT_Vector *control, FT_Vector *to, void *user);
    static int cubicToFunction(const FT_Vector *controlOne,
                               const FT_Vector *controlTwo,
                               const FT_Vector *to,
                               void *user);

public:
    Glyph(const FT_Face &_face, const FT_ULong &_code) : face(_face), code(_code) {}
    ~Glyph() { }

    void parse();
    std::vector<std::vector<glm::vec2>>* getContours();
    void printTest();
};