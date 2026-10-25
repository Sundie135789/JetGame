#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include<GL/gl.h>
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "headers/model.hpp"
#include "headers/loadshader.hpp"
#include "headers/global.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#define HEIGHT 1080
#define WIDTH 1920
#include "stb_image.h"
void loadTexture(const aiScene* scene, aiMaterial* material, GLuint& textureID){
    aiString path;
    bool shouldFree = !true;
    if(material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS){
        const aiTexture* texture = scene->GetEmbeddedTexture(path.C_Str());
        if(texture){
            int width, height, channels;
            unsigned char* data = nullptr;
            if(texture->mHeight == 0){
                data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(texture->pcData), texture->mWidth, &width, &height, &channels, 4);
                shouldFree = true;
            }else{

                int bufferSize=  texture->mWidth * texture->mHeight * sizeof(aiTexel);
                data = stbi_load_from_memory(reinterpret_cast<const unsigned char*>(texture->pcData), bufferSize, &width, &height, &channels ,4);
                shouldFree = true;
            }
            if(data){
                glGenTextures(1, &textureID);
                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);
                if(shouldFree)
                    stbi_image_free(data);
                std::cout << "Successfully loaded texture: " << width << "x" << height << std::endl;
            }else{
                std::cerr << "FAiled to parse image data via stb_image out of gltf asset. \n";
            }
        }else{
            std::cerr <<" texture path found ( " << path.C_Str() << ") but texture was not embedded in FBX\n";
        }
    }
}
void checkShader(GLuint shader, const char* name){
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
        char log[2048];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        std::cerr << name << " shader error:\n" << log << '\n';
    }
}
Model::Model(const std::string &path) {
    std::string vert_text = LoadShader("shaders/basic.vert");
    std::string frag_text = LoadShader("shaders/basic.frag");
    this->loadModel(path);
    // opengl 
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, packedData.size() * sizeof(float), packedData.data(), GL_STATIC_DRAW);
    GLsizei stride = 8 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride,(void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,(void*)(3*sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // shaders
    GLuint vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char* vertex_shader = vert_text.c_str();
    glShaderSource(vertex, 1, &vertex_shader, nullptr);
    const char* fragment_shader = frag_text.c_str();
    glShaderSource(fragment, 1, &fragment_shader, nullptr);
    glCompileShader(vertex);
    checkShader(vertex, "Vertex");
    glCompileShader(fragment);
    checkShader(fragment, "Fragment");
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        char log[2048];
        glGetProgramInfoLog(shaderProgram, sizeof(log), nullptr, log);
        std::cerr << "Shader Program Link Error:\n" << log << '\n'; 
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}
void Model::Render(glm::dmat4 modelMatrix){
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->textureID);
        glUniform1i(glGetUniformLocation(this->shaderProgram, "texture_albedo"),0);
        double s = 0.6;
        modelMatrix = glm::scale(modelMatrix, glm::dvec3(s, s, s));

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(modelMatrix)));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(mainCamera->view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(mainCamera->projection));
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(packedData.size() / 8));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}
void Model::loadModel(const std::string& path){
    Assimp::Importer importer;
    const aiScene* scene=  importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    this->packedData.clear();
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        std::cerr << "Attempted to load path: " << path << std::endl;
        std::exit(1);
    } 
    if(scene && scene->mNumMeshes > 0 && scene->mNumMaterials >  scene->mMeshes[0]->mMaterialIndex){
        loadTexture(scene, scene->mMaterials[scene->mMeshes[0]->mMaterialIndex], this->textureID);
    }
    for(unsigned int i=0;i<scene->mNumMeshes;++i){
        aiMesh* mesh = scene->mMeshes[i];
        for(unsigned int j=0;j<mesh->mNumFaces;++j){
            aiFace face = mesh->mFaces[j];
            if(face.mNumIndices !=3)continue;
            for(unsigned int k =0;k<face.mNumIndices;++k){
                unsigned int index = face.mIndices[k];
                packedData.push_back(mesh->mVertices[index].x);
                packedData.push_back(mesh->mVertices[index].y);
                packedData.push_back(mesh->mVertices[index].z);
                if(mesh->HasNormals()){
                    packedData.push_back(mesh->mNormals[index].x);
                    packedData.push_back(mesh->mNormals[index].y);
                    packedData.push_back(mesh->mNormals[index].z);
                }else{
                    packedData.push_back(0.0f);
                    packedData.push_back(0.0f);
                    packedData.push_back(0.0f);
                }
                if(mesh->mTextureCoords[0]){
                    packedData.push_back(mesh->mTextureCoords[0][index].x);
                    packedData.push_back(mesh->mTextureCoords[0][index].y);
                }else{
                    packedData.push_back(0.0f);
                    packedData.push_back(0.0f);
                }
            }
        }
    }
}
