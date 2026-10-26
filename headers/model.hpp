#pragma once
#include <string>
#include <GL/gl.h>
#include <glm/glm.hpp>
class Model{
    public:
    Model(const std::string& path);
    Model(std::vector<float> packedData);
    void Render(glm::dmat4& modelMatrix);
    void RenderInstanced(const std::vector<glm::mat4>& floatTransforms, int instanceCount);
    void loadModel(const std::string& path);
    //std::vector<float> vertices, normals, uvs;
    std::vector<float> packedData;
    GLuint shaderProgram, vbo, vao, textureID;
    glm::dmat4 orientation;
    unsigned int instanceVBO;
};
