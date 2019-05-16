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

bool TextTessellation::isOverlay(const std::vector<glm::vec2> polygon1, const std::vector<glm::vec2> polygon2)
{
    const glm::vec2 *p0 = &polygon1.at(0);
    const float px0 = (*p0).x;
    const float py0 = (*p0).y;

    float minX = -999999.0f, maxX = 999999.0f, minY = -999999.0f, maxY = 999999.0f;
    for (auto it = polygon2.begin(); it != polygon2.end(); it++)
    {
        const float px = (*it).x;
        const float py = (*it).y;

        minX = std::min(maxX, px);
        minY = std::min(maxY, py);
        maxX = std::max(minX, px);
        maxY = std::max(minY, py);
    }
    return (px0 >= minX) && (px0 <= maxX) && (py0 >= minY) && (py0 <= maxY);
}

void TextTessellation::tessellate(const std::vector<std::vector<glm::vec2>> &contours,
                                  const float depth,
                                  std::vector<glm::vec3> &result)
{
    const float zValue1 = 0.0f;
    const float zValue2 = zValue1 + depth;

    std::vector<glm::vec2> plain;
    std::vector<std::vector<glm::vec2>> polygon;

    std::vector<glm::vec2> c0 = contours.at(0);
    std::vector<glm::vec2> c1 = contours.at(1);

    bool flag0 = this->isOverlay(c0, c1);
    bool flag1 = this->isOverlay(c1, c0);

    std::cout << "flag0: " << flag0 << std::endl;
    std::cout << "flag1: " << flag1 << std::endl;


    // std::cout << "contours.size: " << contours.size() << std::endl;
    // polygon.push_back(contours.at(1));
    // plain = contours.at(1);

    for (auto it = contours.begin(); it != contours.end(); it++)
    {

        // polygon.clear();
        polygon.push_back(*it);
        plain.insert(plain.end(), it->begin(), it->end());
        // std::cout << "it->size: " << it->size() << std::endl;
        // for(auto it2 = it->begin(); it2 != it->end(); it2++)
        // {
        //     plain.push_back(*it2);
        // }
        // break;
    }

    std::vector<unsigned int> indices = mapbox::earcut<unsigned int>(polygon);
    this->fillFrontFace(indices, plain, zValue1, result);
    this->fillSideFace(contours, zValue1, zValue2, result);
    this->fillFrontFace(indices, plain, zValue2, result);
}