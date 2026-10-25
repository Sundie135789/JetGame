#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "headers/global.hpp"
#include "headers/camera.hpp"
Camera::Camera(){
    fov = glm::radians(60.0f);
    aspect = window_width / window_height;
    near = 0.1f;
    far = 500.0f;
    position = glm::vec3(0.0f, 2.0f, -5.0f);
    target = glm::vec3(0.0f, 2.0f, 0.0f);
    view = glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(60.0f), 1920.0f/1080.0f, 0.1f, 1000.0f);
}
