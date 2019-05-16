#pragma once

#include "../../app.hpp"
#include "../../../include/mapbox/earcut.hpp"

namespace mapbox
{
namespace util
{

template <>
struct nth<0, glm::vec2>
{
    inline static auto get(const glm::vec2 &t)
    {
        return t.x;
    };
};
template <>
struct nth<1, glm::vec2>
{
    inline static auto get(const glm::vec2 &t)
    {
        return t.y;
    };
};

} // namespace util
} // namespace mapbox

class TextTessellation
{
public:
    TextTessellation() {}
    ~TextTessellation() {}

    void tessellate(const std::vector<std::vector<glm::vec2>> &contours,
                    const float depth,
                    std::vector<glm::vec3> &result);

private:
    bool isOverlay(const std::vector<glm::vec2> polygon1, const std::vector<glm::vec2> polygon2);

    void fillFrontFace(const std::vector<unsigned int> &indices,
                       const std::vector<glm::vec2> &contoursPoints2d,
                       const float zValue,
                       std::vector<glm::vec3> &result);

    void fillSideFace(const std::vector<std::vector<glm::vec2>> &contoursPoints2d,
                      const float zValue1,
                      const float zValue2,
                      std::vector<glm::vec3> &result);
};