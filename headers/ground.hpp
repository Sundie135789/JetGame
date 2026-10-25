#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
class Ground{
    private:
        unsigned int vao, vbo ,ebo;
        unsigned int shaderProgram;
        int indexCount;
        void CompileShader();
    public:
        Ground(float size, float height);
        ~Ground();
        void Draw();
};