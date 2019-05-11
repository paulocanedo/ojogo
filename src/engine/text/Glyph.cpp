#include "Glyph.hpp"

void Glyph::load(const FT_Face &_face, const FT_ULong &_code)
{
    FT_UInt index = FT_Get_Char_Index(_face, _code);
    FT_Error error = FT_Load_Glyph(face,
                                   index,
                                   FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP);

    if (error)
    {
        throw std::runtime_error("Couldn't load the glyph: FT_Load_Glyph() failed");
    }
}

void Glyph::parse()
{
    this->load(this->face, this->code);
    FT_Outline_Funcs callbacks;

    callbacks.move_to = (FT_Outline_MoveTo_Func)&moveToFunction;
    callbacks.line_to = (FT_Outline_LineTo_Func)&lineToFunction;
    callbacks.conic_to = (FT_Outline_ConicTo_Func)&conicToFunction;
    callbacks.cubic_to = (FT_Outline_CubicTo_Func)&cubicToFunction;

    callbacks.shift = 0;
    callbacks.delta = 0;

    FT_GlyphSlot slot = face->glyph;
    FT_Outline &outline = slot->outline;

    FT_Error error = FT_Outline_Decompose(&outline, &callbacks, this);
    if (error)
    {
        throw std::runtime_error("Couldn't extract the outline: FT_Outline_Decompose() failed");
    }

    // FT_BBox boundingBox;
    // FT_Outline_Get_BBox(&outline, &boundingBox);

    // float xMin = boundingBox.xMin;
    // float yMin = boundingBox.yMin;
    // float xMax = boundingBox.xMax;
    // float yMax = boundingBox.yMax;
}

std::vector<std::vector<glm::vec2>> *Glyph::getContours() {
    return &(this->contours);
}

void Glyph::printTest()
{
    std::vector<std::vector<glm::vec2>> *contours = &(this->contours);
    for (std::vector<std::vector<glm::vec2>>::iterator oit = contours->begin(); oit != contours->end(); oit++)
    {
        std::cout << "contour size: " << oit->size() << std::endl;
        for (std::vector<glm::vec2>::iterator iit = oit->begin(); iit != oit->end(); iit++)
        {
            std::cout << (*iit).x << "," << (*iit).y << std::endl;
        }
    }
    std::cout << "vector size: " << contours->size() << std::endl;
}

int Glyph::moveToFunction(FT_Vector *to, void *user)
{
    Glyph *glyph = static_cast<Glyph *>(user);
    std::vector<glm::vec2> contour;

    contour.push_back({to->x, to->y});
    glyph->contours.push_back(contour);

    return 0;
}

int Glyph::lineToFunction(FT_Vector *to, void *user)
{
    Glyph *glyph = static_cast<Glyph *>(user);

    auto *lastContour = &(glyph->contours.end()[-1]);
    lastContour->push_back({to->x, to->y});

    return 0;
}

int Glyph::conicToFunction(FT_Vector *control, FT_Vector *to, void *user)
{
    Glyph *glyph = static_cast<Glyph *>(user);

    auto *lastContour = &(glyph->contours.end()[-1]);
    glm::vec2 *lastElem = &(lastContour->end()[-1]);

    float px0 = lastElem->x;
    float py0 = lastElem->y;

    float px1 = control->x;
    float py1 = control->y;

    float px2 = to->x;
    float py2 = to->y;

    for (float delta = 0.0f; delta <= 1.0f; delta += BEZIER_STEP)
    {
        float x = pow(1 - delta, 2) * px0 + (1 - delta) * 2 * delta * px1 + delta * delta * px2;
        float y = pow(1 - delta, 2) * py0 + (1 - delta) * 2 * delta * py1 + delta * delta * py2;

        lastContour->push_back({x, y});
    }

    return 0;
}

int Glyph::cubicToFunction(const FT_Vector *controlOne,
                           const FT_Vector *controlTwo,
                           const FT_Vector *to,
                           void *user)
{
    Glyph *glyph = static_cast<Glyph *>(user);

    auto *lastContour = &(glyph->contours.end()[-1]);
    glm::vec2 *lastElem = &(lastContour->end()[-1]);

    for (float delta = 0.0f; delta <= 1.0f; delta += BEZIER_STEP)
    {
        float x = pow(1 - delta, 3) * lastElem->x + pow(1 - delta, 2) * 3 * delta * controlOne->x + (1 - delta) * 3 * delta * delta * controlTwo->x + delta * delta * delta * to->x;
        float y = pow(1 - delta, 3) * lastElem->y + pow(1 - delta, 2) * 3 * delta * controlOne->y + (1 - delta) * 3 * delta * delta * controlTwo->y + delta * delta * delta * to->y;

        lastContour->push_back({x, y});
    }

    return 0;
}