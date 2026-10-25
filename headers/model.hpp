#pragma once
#include <string>
#include <GL/gl.h>
#include <glm/glm.hpp>
class Model{
    public:
    Model(const std::string& path);
    void Render(glm::dmat4 jetOrientation);
    void loadModel(const std::string& path);
    //std::vector<float> vertices, normals, uvs;
    std::vector<float> packedData;
    GLuint shaderProgram, vbo, vao, textureID;

};