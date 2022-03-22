#include <kc/cv/Image.h>

#include <iostream>
#include <limits>

using namespace kc;

struct Triangle {
    glm::vec3 v0, v1, v2, color;
};

void projectVertices(Triangle& triangle, float near, const cv::Image::Size& canvas) {
    auto& t = triangle;
    t.v0.z = -t.v0.z;
    t.v0.x = (near * t.v0.x / t.v0.z);
    t.v0.y = (near * t.v0.y / t.v0.z);

    t.v1.z = -t.v1.z;
    t.v1.x = (near * t.v1.x / t.v1.z);
    t.v1.y = (near * t.v1.y / t.v1.z);

    t.v2.z = -t.v2.z;
    t.v2.x = (near * t.v2.x / t.v2.z);
    t.v2.y = (near * t.v2.y / t.v2.z);

    auto [width, height] = canvas;

    t.v0.y = (1 - t.v0.y / height) * height;
    t.v1.y = (1 - t.v1.y / height) * height;
    t.v2.y = (1 - t.v2.y / height) * height;
}

inline float edgeFunction(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
    return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
}

int main() {
    cv::Image image{cv::Image::Size{.width = 600, .height = 600}};
    auto [w, h] = image.getSize();

    std::vector<float> zBuffer(w * h, std::numeric_limits<float>::max());

    std::vector<Triangle> triangles = {
        Triangle{
            .v0 = {100, 0, -1}, .v1 = {600, 0, -1}, .v2 = {350, 600, -1}, .color = {1.0, 1.0, 0.0}},
        Triangle{.v0 = {0, 0, -1.5},
                 .v1 = {300, 0, -1.5},
                 .v2 = {150, 600, -1.5},
                 .color = {1.0, 0.0, 0.0}}};

    const float near = 1.0f;

    for (auto triangle : triangles) {
        projectVertices(triangle, near, image.getSize());

        float area = edgeFunction(triangle.v0, triangle.v1, triangle.v2);

        for (int x = 0; x < w; ++x) {
            for (int y = 0; y < h; ++y) {
                glm::vec3 p{x + 0.5, y + 0.5, 0.0f};

                float w0 = edgeFunction(triangle.v1, triangle.v2, p);
                float w1 = edgeFunction(triangle.v2, triangle.v0, p);
                float w2 = edgeFunction(triangle.v0, triangle.v1, p);

                if (w0 > 0 && w1 > 0 && w2 > 0) {
                    int zIndex = y * h + x;

                    if (zBuffer[zIndex] >= triangle.v1.z) {
                        zBuffer[zIndex] = triangle.v1.z;
                        image.setPixelValue(x, y, triangle.color);
                    }
                }
            }
        }
    }

    image.save("test.jpg");

    return 0;
}