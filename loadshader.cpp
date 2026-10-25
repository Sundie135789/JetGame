#include "headers/loadshader.hpp"
#include <iostream>
std::string LoadShader(const std::string& path){
    std::ifstream file(path);
    if(!file.is_open()){
        std::cerr << "Failed to open shader: " << path << '\n';
        return "";
    }
    std::string file_text, line;
    while(getline(file, line))
        file_text += line + '\n';
    
    return file_text;
}