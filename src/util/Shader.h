//
// Created by Tykhon Korol on 15.12.25.
//

#ifndef SHADERLINKER_H
#define SHADERLINKER_H
#include <string>
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include "../thirdparty/glad/glad.h"

class Shader {
    unsigned int ID;

    std::string loadShaderSource(const std::string &filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Failed to open " << filepath << "\n";
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    unsigned int createFragmentShader(std::string const& filepath) {
        const std::string shader_source = loadShaderSource(filepath);
        const char *fSrc = shader_source.c_str();

        const unsigned int shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(shader, 1, &fSrc, nullptr);
        int success;
        char info_log[512];
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, info_log);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
        }
        return shader;
    }

    unsigned int createVertexShader(std::string const& filepath) {
        const std::string shader_source = loadShaderSource(filepath);
        const char *fSrc = shader_source.c_str();

        const unsigned int shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(shader, 1, &fSrc, nullptr);
        int success;
        char info_log[512];
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, info_log);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
        }
        return shader;
    }

    unsigned int linkShaders(unsigned int vertex, unsigned int fragment) {
        unsigned int shader_programm = glCreateProgram();
        glAttachShader(shader_programm, vertex);
        glAttachShader(shader_programm, fragment);
        glLinkProgram(shader_programm);

        int success;
        char info_log[512];
        glGetProgramiv(shader_programm, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader_programm, 512, nullptr, info_log);
            std::cout << "ERROR::LINKING::SHADERS\n" << info_log << std::endl;
        }
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return shader_programm;
    }

public:
    explicit Shader(std::string const& filepath_vertex_sh, std::string const& filepath_fragment_sh) {
        ID = linkShaders(createVertexShader(filepath_vertex_sh),
                         createFragmentShader(filepath_fragment_sh));
    }

    void use() const {
        glUseProgram(ID);
    }

    unsigned int getID() {
        return ID;
    }

    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
    }

    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void setVec2(const std::string &name, float x, float y) const {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }

    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void setVec3(const std::string &name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }

    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void setVec4(const std::string &name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }

    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};
#endif //SHADERLINKER_H
