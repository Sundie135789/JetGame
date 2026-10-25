#include <GL/glew.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <vector>
#include <string>
// render the ground bruv
#include "headers/model.hpp"
#include "headers/camera.hpp"
#include "headers/global.hpp"
#define HEIGHT 1080
#define WIDTH 1920
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
//add ground
// add lighting pbr
glm::dmat4 jetOrientation = glm::dmat4(1.0f);
double rollSpeed = 0.6;
double pitchSpeed = 0.3;
double yawSpeed = 0.15;
double jetSpeed = 0.1;
double deltaTime = 1.0, lastTime =0.0;
void processInput(GLFWwindow* window){
    glm::dvec3 twist(
        (glfwGetKey(window, GLFW_KEY_W) - glfwGetKey(window, GLFW_KEY_S)) * pitchSpeed * deltaTime,
        (glfwGetKey(window, GLFW_KEY_Q) - glfwGetKey(window, GLFW_KEY_E)) * yawSpeed * deltaTime,
        (glfwGetKey(window, GLFW_KEY_D) - glfwGetKey(window, GLFW_KEY_A)) *rollSpeed * deltaTime 
    );
    if(glm::length2(twist) > 0.0f){
        jetOrientation= glm::rotate(jetOrientation, glm::length(twist), glm::normalize(twist));
    }
    jetOrientation = glm::translate(jetOrientation, glm::dvec3(0.0, 0.0, jetSpeed));

}
int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "War Thunder - x64", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glViewport(0,0,WIDTH,HEIGHT);



    mainCamera = new Camera();
    Model* model = new Model("assets/Files/F-16D.gltf");
    while(!glfwWindowShouldClose(window)){
        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        if(deltaTime > 0.1) deltaTime = 0.1;
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(0);
        processInput(window);
        model->Render(jetOrientation);
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
