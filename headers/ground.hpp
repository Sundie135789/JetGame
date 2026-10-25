#pragma once
#include <GLFW/glfw3.hpp>
#include <glm/glm.hpp>
class Ground{
    private:
        unsigned int vao, vbo ,ebo;
        unsigned int shaderProgram;
        int indexCount;
        void CompileShader();
        void SetupMesh();
    public:
        Ground(float size, float height);
        ~Ground();
        void Draw(const glm::mat4& view, const glm::mat4& projection);
};