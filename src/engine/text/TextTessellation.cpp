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

bool TextTessellation::isOverlay(const std::vector<glm::vec2> *polygon1, const std::vector<glm::vec2> *polygon2)
{
    const glm::vec2 *p0 = &polygon1->at(0);
    const float px0 = p0->x;
    const float py0 = p0->y;

    float minX = 999999.0f, maxX = -999999.0f, minY = 999999.0f, maxY = -999999.0f;
    for (auto it = polygon2->begin(); it != polygon2->end(); it++)
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
    for (size_t i = 0; i < input.size(); i++)
    {
        for (size_t j = i; j < input.size(); j++)
        {
            if (i == j)
                continue;

            std::vector<glm::vec2> *poly1 = &(result.at(i));
            std::vector<glm::vec2> *poly2 = &(result.at(j));

            if (this->isOverlay(poly1, poly2))
            {
                iter_swap(result.begin() + i, result.begin() + j);
            }
        }
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

std::pair<std::vector<std::vector<glm::vec2>>, std::vector<std::vector<glm::vec2>>> TextTessellation::classifyContours(std::vector<std::vector<glm::vec2>> input)
{
    std::pair<std::vector<std::vector<glm::vec2>>, std::vector<std::vector<glm::vec2>>> newContours;
    std::vector<std::vector<glm::vec2>> overlaid;
    std::vector<std::vector<glm::vec2>> nonOverlaid;

    for (size_t i = 0; i < input.size(); i++)
    {
        std::vector<glm::vec2> *contour1 = &(input.at(i));

        bool overlays = false;

        for (size_t j = 0; j < input.size(); j++)
        {
            if (i == j)
                continue;

            std::vector<glm::vec2> *contour2 = &(input.at(j));
            if (this->isOverlay(contour1, contour2) || this->isOverlay(contour2, contour1))
            {
                overlays = true;
                break;
            }
        }

        if (overlays)
        {
            overlaid.push_back(*contour1);
        }
        else
        {
            nonOverlaid.push_back(*contour1);
        }
    }

    newContours.first = overlaid;
    newContours.second = nonOverlaid;
    return newContours;
}

void TextTessellation::tessellate(const std::vector<std::vector<glm::vec2>> &contours,
                                  const float depth,
                                  std::vector<glm::vec3> &result)
{
    const float zValue1 = 0.0f;
    const float zValue2 = zValue1 + depth;

    std::vector<glm::vec2> plain;
    std::vector<std::vector<glm::vec2>> polygon;

    std::vector<std::vector<glm::vec2>> pContours = this->prepareForTriangulation(contours);
    std::pair<std::vector<std::vector<glm::vec2>>, std::vector<std::vector<glm::vec2>>> classifiedContours = this->classifyContours(pContours);

    //overlaid contours
    for (auto it = classifiedContours.first.begin(); it != classifiedContours.first.end(); it++)
    {
        polygon.push_back(*it);
        plain.insert(plain.end(), it->begin(), it->end());
    }
    std::vector<unsigned int> indices = mapbox::earcut<unsigned int>(polygon);
    this->fillFrontFace(indices, plain, zValue1, result);
    this->fillSideFace(contours, zValue1, zValue2, result);
    this->fillFrontFace(indices, plain, zValue2, result);

    //non overlaid contours
    for (auto it = classifiedContours.second.begin(); it != classifiedContours.second.end(); it++)
    {
        polygon.clear();
        polygon.push_back(*it);

        indices = mapbox::earcut<unsigned int>(polygon);
        this->fillFrontFace(indices, *it, zValue1, result);
        this->fillSideFace(contours, zValue1, zValue2, result);
        this->fillFrontFace(indices, *it, zValue2, result);
    }
}