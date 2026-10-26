/*
 
TECHNOLOGIES:
Switched from float matrices / vectors to double matrices / vectors to prevent matrix drift for about 7 days on 500 FPS
Switched from 1x3 vector for rotation and position, to one 4x4 matrix containing both.
Used persistent storage of global transform matrix (except scale) instead of quaternions.
Added Polyform noncommercial license 1.0.0

OPTIMIZATION:
Instancing of One thousand trees
Removed std::cout for Deltatime print

*/
/*
 TO-DO LIST:
	1. Do correct instances, atleast 100 FPS with 1000 Loop Count.
	2. Chunk Loading
 */
#include <GL/glew.h>
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <glm/gtx/norm.hpp>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <vector>
#include <string>
#include "headers/model.hpp"
#include "headers/camera.hpp"
#include "headers/global.hpp"
#include "headers/ground.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
// rotate the tree, put manby of them
Model* jet;
double rollSpeed = 0.6;
double pitchSpeed = 0.5;
double yawSpeed = 0.1;
double jetSpeed = 1.0;
double deltaTime = 1.0, lastTime =0.0;
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    window_width = width;
    window_height = height;
    glViewport(0, 0, window_width, window_height);
}
void processInput(GLFWwindow* window){
    glm::dvec3 twist(
        (glfwGetKey(window, GLFW_KEY_W) - glfwGetKey(window, GLFW_KEY_S)) * pitchSpeed * deltaTime,
        (glfwGetKey(window, GLFW_KEY_Q) - glfwGetKey(window, GLFW_KEY_E)) * yawSpeed * deltaTime,
        (glfwGetKey(window, GLFW_KEY_D) - glfwGetKey(window, GLFW_KEY_A)) *rollSpeed * deltaTime 
    );
    if(glm::length2(twist) > 0.0f){
        jet->orientation= glm::rotate(jet->orientation, glm::length(twist), glm::normalize(twist));
    }
    jet->orientation = glm::translate(jet->orientation, glm::dvec3(0.0, 0.0, jetSpeed));

}
std::vector<glm::mat4> treeTransformsFloat;
int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "War Thunder - x64", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glViewport(0,0,window_width, window_height);


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    std::vector<Model*> models; 
    jet = new Model("assets/Files/F-16D.gltf");
    mainCamera = new Camera();
    Ground *ground = new Ground(1000.0f, -5.0f);
    Model* initial = new Model("assets/Tree.fbx");
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distr(-100.0f, 100.0f);
    for(int i=0; i< 100 ;i++){

				glm::dmat4 transform = glm::dmat4(1.0f);
        float randomXDeviation = distr(gen), randomZDeviation = distr(gen);
       	transform= glm::translate(transform, glm::dvec3(randomXDeviation, -5.0f, randomZDeviation));
        transform = glm::rotate(transform, glm::radians(-90.0), glm::dvec3(1.0f, 0.0f, 0.0f));
	double s = 0.6;

	glm::dmat4 scaledMatrix = glm::scale(transform, glm::dvec3(s, s, s));
        treeTransformsFloat.push_back(glm::mat4(scaledMatrix));
    }
    glBindBuffer(GL_ARRAY_BUFFER, initial->instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, treeTransformsFloat.size() * sizeof(glm::mat4), treeTransformsFloat.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0 );
    while(!glfwWindowShouldClose(window)){
        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        if(deltaTime > 0.1) deltaTime = 0.1;
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);
        ground->Draw();
        jet->Render(jet->orientation);
	// rendering trees
	initial->RenderInstanced(treeTransformsFloat, treeTransformsFloat.size());
        mainCamera->Follow(jet->orientation);
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
