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
                                     const std::vector<std::vector<Point>> &contoursPoints2d,
                                     const float zValue,
                                     std::vector<glm::vec3> &result)
{
    for (size_t i = 0; i < indices.size(); i++)
    {
        N index = indices[i];
        const Point *p = &contoursPoints2d.at(0).at(index);

        result.push_back(glm::vec3(std::get<0>(*p), std::get<1>(*p), zValue));
        result.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    }
}

void TextTessellation::fillSideFace(const std::vector<N> &indices,
                                    const std::vector<std::vector<Point>> &contoursPoints2d,
                                    const float zValue1,
                                    const float zValue2,
                                    std::vector<glm::vec3> &result)
{
    for (size_t i = 0; i < contoursPoints2d.at(0).size(); i++)
    {
        const Point *pp0 = &contoursPoints2d.at(0).at(i + 0);
        const Point *pp1 = &contoursPoints2d.at(0).at(i == contoursPoints2d.at(0).size() - 1 ? 0 : i + 1);

        glm::vec3 p1 = glm::vec3(std::get<0>(*pp0), std::get<1>(*pp0), zValue1);
        glm::vec3 p2 = glm::vec3(std::get<0>(*pp0), std::get<1>(*pp0), zValue2);
        glm::vec3 p3 = glm::vec3(std::get<0>(*pp1), std::get<1>(*pp1), zValue1);
        glm::vec3 p4 = glm::vec3(std::get<0>(*pp1), std::get<1>(*pp1), zValue2);

        glm::vec3 vp1 = p3 - p1;
        glm::vec3 vp2 = p4 - p2;
        // glm::vec3 normal = glm::normalize(glm::cross(vp1, vp2));
        glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);

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

std::vector<glm::vec3> TextTessellation::tessellate(const std::vector<std::vector<glm::vec2>> &contours, const float depth)
{
    std::vector<std::vector<Point>> contoursPoints2d = this->convertToPoint2D(contours);
    std::vector<glm::vec3> contoursPoints3d;
    const float zValue1 = 0.0f;
    const float zValue2 = zValue1 - depth;

    std::vector<std::vector<Point>> polygon;
    polygon.push_back(contoursPoints2d.at(0));

    std::vector<N> indices = mapbox::earcut<N>(polygon);
    this->fillFrontFace(indices, contoursPoints2d, zValue1, contoursPoints3d);
    this->fillSideFace(indices, contoursPoints2d, zValue1, zValue2, contoursPoints3d);
    this->fillFrontFace(indices, contoursPoints2d, zValue2, contoursPoints3d);

    return contoursPoints3d;
}