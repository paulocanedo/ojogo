#pragma once

#include "../../app.hpp"
#include "TextTessellation.hpp"

#define BEZIER_STEP 1 / 10.0f

class Glyph
{
private:
    unsigned int vbo = -1;
    unsigned int vao = -1;

    const FT_Face &face;
    const FT_ULong &code;

    float zValue = 0.0f;
    std::vector<std::vector<glm::vec2>> contours;
    std::vector<glm::vec3> contoursPoints3d;

    void load(const FT_Face &_face, const FT_ULong &_code);

    static int moveToFunction(FT_Vector *to, void *user);
    static int lineToFunction(FT_Vector *to, void *user);
    static int conicToFunction(FT_Vector *control, FT_Vector *to, void *user);
    static int cubicToFunction(const FT_Vector *controlOne,
                               const FT_Vector *controlTwo,
                               const FT_Vector *to,
                               void *user);

public:
    Glyph(const FT_Face &_face, const FT_ULong &_code) : face(_face), code(_code) { this->parse(); }
    ~Glyph() {}

    void parse();
    const std::vector<std::vector<glm::vec2>> *getContours();
    void gpuUpload();
    void gpuDraw();
    void gpuFreeResources();

    void printTest();
};