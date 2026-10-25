#include <GL/glew.h>
#include <GL/gl.h>
#include "headers/ground.hpp"
#include "headers/global.hpp"
#include "headers/loadshader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Ground::Ground(float size, float height){
    CompileShader();
    float halfSize = size / 2.0f;
    float vertices[] = {
        -halfSize, height, -halfSize, 0.2f, 0.5f,  0.2f,
        halfSize, height, -halfSize, 0.2f, 0.6f, 0.2f,
        halfSize, height, halfSize, 0.3f, 0.7f, 0.3f,
        -halfSize, height, halfSize, 0.2f, 0.5f, 0.2f
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    indexCount = 6;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1,&vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}
Ground::~Ground(){
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shaderProgram);
}
void Ground::CompileShader(){
    unsigned int vertex, fragment;
    std::string vertSourceStr = LoadShader("shaders/ground.vert");
    std::string fragSourceStr = LoadShader("shaders/ground.frag");
    
    const char* vertex_cstr = vertSourceStr.c_str();
    const char* fragment_cstr = fragSourceStr.c_str();

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_cstr, NULL);
    glCompileShader(vertex);

    // Initialize, feed source, and compile fragment coloring block
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_cstr, NULL);
    glCompileShader(fragment);

    // Link the compiled program structures together
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);

    // Clean up temporary compilation artifacts
    glDeleteShader(vertex);
    glDeleteShader(fragment);
   
}
void Ground::Draw() {
    glUseProgram(shaderProgram);

    // The ground remains static at the global world coordinate matrix space
    glm::mat4 model = glm::mat4(1.0f);

    // Send uniform matrices directly to the GPU shader registers
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(mainCamera->view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(mainCamera->projection));

    // Execute drawing instructions
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}