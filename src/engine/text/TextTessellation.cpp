#include "TextTessellation.hpp"

std::vector<std::vector<Point>> TextTessellation::convertToPoint2D(const std::vector<std::vector<glm::vec2>> &contours)
{
    std::vector<std::vector<Point>> contoursPoints2d;
    for (auto it = contours.begin(); it != contours.end(); it++)
    {
        std::vector<Point> points2d;

        for (size_t i = 0; i < it->size(); i++)
        {
            points2d.push_back({it->at(i).x, it->at(i).y});
        }

        contoursPoints2d.push_back(points2d);
    }

    return contoursPoints2d;
}

void TextTessellation::fillFrontFace(const std::vector<N> &indices,
                                     const std::vector<Point> &contourPoints2d,
                                     const float zValue,
                                     std::vector<glm::vec3> &result)
{
    for (size_t i = 0; i < indices.size(); i++)
    {
        N index = indices[i];
        const Point *p = &contourPoints2d.at(index);
        const glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f);

        result.push_back(glm::vec3(std::get<0>(*p), std::get<1>(*p), zValue));
        result.push_back(normal);
    }
}

void TextTessellation::fillSideFace(const std::vector<N> &indices,
                                    const std::vector<Point> &contourPoints2d,
                                    const float zValue1,
                                    const float zValue2,
                                    std::vector<glm::vec3> &result)
{
    const int n = contourPoints2d.size() - 1;
    for (size_t i = 0; i < contourPoints2d.size(); i++)
    {
        const Point *pp0 = &contourPoints2d.at(i + 0);
        const Point *pp1 = &contourPoints2d.at((i + 1) % n);

        glm::vec3 p1 = glm::vec3(std::get<0>(*pp0), std::get<1>(*pp0), zValue1);
        glm::vec3 p2 = glm::vec3(std::get<0>(*pp0), std::get<1>(*pp0), zValue2);
        glm::vec3 p3 = glm::vec3(std::get<0>(*pp1), std::get<1>(*pp1), zValue1);
        glm::vec3 p4 = glm::vec3(std::get<0>(*pp1), std::get<1>(*pp1), zValue2);

        glm::vec3 vp1 = p2 - p1;
        glm::vec3 vp2 = p3 - p2;
        glm::vec3 normal = glm::normalize(glm::cross(vp1, vp2));
        // glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);

        result.push_back(p1);
        result.push_back(normal);
        result.push_back(p2);
        result.push_back(normal);
        result.push_back(p3);
        result.push_back(normal);

        result.push_back(p2);
        result.push_back(normal);
        result.push_back(p3);
        result.push_back(normal);
        result.push_back(p4);
        result.push_back(normal);
    }
}

bool TextTessellation::isOverlay(const std::vector<Point> polygon1, const std::vector<Point> polygon2)
{
    const Point *p0 = &polygon1.at(0);
    const float px0 = std::get<0>(*p0);
    const float py0 = std::get<1>(*p0);

    float minX = -999999.0f, maxX = 999999.0f, minY = -999999.0f, maxY = 999999.0f;
    for(auto it = polygon2.begin(); it != polygon2.end(); it++)
    {
        const float px = std::get<0>(*it);
        const float py = std::get<1>(*it);

        minX = std::min(minX, px);
        minY = std::min(minY, py);
        maxX = std::max(maxX, px);
        maxY = std::max(maxY, py);
    }
    return (px0 > minX) && (px0 < maxX) && (py0 > minY) && (py0 < maxY);
}

void TextTessellation::tessellate(const std::vector<std::vector<glm::vec2>> &contours,
                                  const float depth,
                                  std::vector<glm::vec3> &result)
{
    std::vector<std::vector<Point>> contoursPoints2d = this->convertToPoint2D(contours);
    const float zValue1 = 0.0f;
    const float zValue2 = zValue1 + depth;

    std::vector<std::vector<Point>> polygon;
    for (auto it = contoursPoints2d.begin(); it != contoursPoints2d.end(); it++)
    {
        
        polygon.clear();
        polygon.push_back(*it);

        std::vector<N> indices = mapbox::earcut<N>(polygon);
        this->fillFrontFace(indices, *it, zValue1, result);
        this->fillSideFace(indices, *it, zValue1, zValue2, result);
        this->fillFrontFace(indices, *it, zValue2, result);
    }
}