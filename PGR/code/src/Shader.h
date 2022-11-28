//----------------------------------------------------------------------------------------
/**
 * \file    Shader.h
 * \author  Veronika Shirochenkova
 * \date    2022
 * \brief   Shader class implementation; shader loading/compilation
 */
 //----------------------------------------------------------------------------------------

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
    public:
        unsigned int ID;

        /* Constructor */
        Shader(const char* vertexPath, const char* fragmentPath)
        {

            std::string vertexCode;
            std::string fragmentCode;

            std::ifstream vShaderFile;
            std::ifstream fShaderFile;

            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();

            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();


            unsigned int vertex, fragment;

            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);

            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);

            // shader Program
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);

            glLinkProgram(ID);

            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }


        void use()
        {
            glUseProgram(ID);
        }

};
#endif
