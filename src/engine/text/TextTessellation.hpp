#pragma once

#include "../../app.hpp"
#include "../../../include/mapbox/earcut.hpp"

using Coord = float;
using N = uint32_t;
using Point = std::array<Coord, 2>;

class TextTessellation
{
public:
    TextTessellation() {}
    ~TextTessellation() {}

    std::vector<glm::vec3> tessellate(const std::vector<std::vector<glm::vec2>> &contours, const float depth = 100.0f);

private:
    std::vector<std::vector<Point>> convertToPoint2D(const std::vector<std::vector<glm::vec2>> &contours);

    void fillFrontFace(const std::vector<N> &indices,
                       const std::vector<std::vector<Point>> &contoursPoints2d,
                       const float zValue,
                       std::vector<glm::vec3> &result);

    void fillSideFace(const std::vector<N> &indices,
                      const std::vector<std::vector<Point>> &contoursPoints2d,
                      const float zValue1,
                      const float zValue2,
                      std::vector<glm::vec3> &result);
};