#include "TextTessellation.hpp"

void TextTessellation::fillFrontFace(const std::vector<unsigned int> &indices,
                                     const std::vector<glm::vec2> &contourPoints2d,
                                     const float zValue,
                                     std::vector<glm::vec3> &result)
{
    for (size_t i = 0; i < indices.size(); i++)
    {
        unsigned int index = indices[i];
        const glm::vec2 *p = &contourPoints2d.at(index);
        const glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f);

        result.push_back(glm::vec3(*p, zValue));
        result.push_back(normal);
    }
}

void TextTessellation::fillSideFace(const std::vector<std::vector<glm::vec2>> &contoursPoints2d,
                                    const float zValue1,
                                    const float zValue2,
                                    std::vector<glm::vec3> &result)
{
    for (auto it = contoursPoints2d.begin(); it != contoursPoints2d.end(); it++)
    {

        const int n = it->size() - 1;
        for (size_t i = 0; i < it->size(); i++)
        {
            const glm::vec2 *pp0 = &it->at((i + 0) % n);
            const glm::vec2 *pp1 = &it->at((i + 1) % n);

            glm::vec3 p1 = glm::vec3((*pp0).x, (*pp0).y, zValue1);
            glm::vec3 p2 = glm::vec3((*pp0).x, (*pp0).y, zValue2);
            glm::vec3 p3 = glm::vec3((*pp1).x, (*pp1).y, zValue1);
            glm::vec3 p4 = glm::vec3((*pp1).x, (*pp1).y, zValue2);

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
}

bool TextTessellation::isOverlay(const std::vector<glm::vec2> &polygon1, const std::vector<glm::vec2> &polygon2)
{
    const glm::vec2 *p0 = &polygon1.at(0);
    const float px0 = p0->x;
    const float py0 = p0->y;

    float minX = 999999.0f, maxX = -999999.0f, minY = 999999.0f, maxY = -999999.0f;
    for (auto it = polygon2.begin(); it != polygon2.end(); it++)
    {
        const float px = it->x;
        const float py = it->y;

        minX = std::min(minX, px);
        minY = std::min(minY, py);
        maxX = std::max(maxX, px);
        maxY = std::max(maxY, py);
    }
    return (px0 >= minX) && (px0 <= maxX) && (py0 >= minY) && (py0 <= maxY);
}

std::vector<std::vector<glm::vec2>> TextTessellation::prepareForTriangulation(const std::vector<std::vector<glm::vec2>> &input)
{
    if (input.size() == 1)
        return input;

    std::vector<std::vector<glm::vec2>> result = input;
    std::vector<glm::vec2> poly1 = input.at(0);
    std::vector<glm::vec2> poly2 = input.at(1);

    if (this->isOverlay(poly1, poly2))
    {
        std::reverse(result.begin(), result.end());
    }
    
    return result;
}

std::vector<glm::vec2> TextTessellation::mergePolygons(const std::vector<glm::vec2> polygon1,
                                                       const std::vector<glm::vec2> polygon2)
{
    std::vector<glm::vec2> result;
    result.insert(result.end(), polygon1.begin(), polygon1.end());
    result.insert(result.end(), polygon2.begin(), polygon2.end());

    return result;
}

void TextTessellation::tessellate(const std::vector<std::vector<glm::vec2>> &contours,
                                  const float depth,
                                  std::vector<glm::vec3> &result)
{
    const float zValue1 = 0.0f;
    const float zValue2 = zValue1 + depth;

    std::vector<glm::vec2> plain;
    std::vector<std::vector<glm::vec2>> polygon;
    // std::vector<std::vector<glm::vec2>> preparedContours = contours;
    std::vector<std::vector<glm::vec2>> preparedContours = this->prepareForTriangulation(contours);
    for (auto it = preparedContours.begin(); it != preparedContours.end(); it++)
    {
        // polygon.clear();
        polygon.push_back(*it);
        plain.insert(plain.end(), it->begin(), it->end());
    }
    std::vector<unsigned int> indices = mapbox::earcut<unsigned int>(polygon);
    this->fillFrontFace(indices, plain, zValue1, result);
    this->fillSideFace(contours, zValue1, zValue2, result);
    this->fillFrontFace(indices, plain, zValue2, result);
}