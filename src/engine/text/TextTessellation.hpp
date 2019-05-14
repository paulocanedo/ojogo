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

    void tessellate(const std::vector<std::vector<glm::vec2>> &contours,
                    const float depth,
                    std::vector<glm::vec3> &result);

private:
    std::vector<std::vector<Point>> convertToPoint2D(const std::vector<std::vector<glm::vec2>> &contours);
    bool isOverlay(const std::vector<Point> polygon1, const std::vector<Point> polygon2);

    void fillFrontFace(const std::vector<N> &indices,
                       const std::vector<Point> &contoursPoints2d,
                       const float zValue,
                       std::vector<glm::vec3> &result);

    void fillSideFace(const std::vector<N> &indices,
                      const std::vector<Point> &contoursPoints2d,
                      const float zValue1,
                      const float zValue2,
                      std::vector<glm::vec3> &result);
};