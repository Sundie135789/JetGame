#pragma once
#include <glm/glm.hpp>
class Camera{
    public:
    Camera();
    float fov, aspect, near, far;
    glm::mat4 view, projection;
    glm::vec3 position, target;
};