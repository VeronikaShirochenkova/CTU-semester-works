//----------------------------------------------------------------------------------------
/**
 * \file    Mesh.h
 * \author  Veronika Shirochenkova
 * \date    2022
 * \brief   Mesh class implementation; Buffers binding/drawing meshes functions
 */
 //----------------------------------------------------------------------------------------

#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <vector>

struct Vertex {
    glm::vec3 Position; 
    glm::vec3 Normal;   
    glm::vec2 TexCoords;
    glm::vec3 Tangent;  
    glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
    public:
        std::vector<Vertex>       cubeVertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;

        // buffers
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;

        /* Constructor */
        Mesh(std::vector<Vertex> cubeVertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
        {
            this->cubeVertices = cubeVertices;
            this->indices = indices;
            this->textures = textures;

            create();
        }

        /* draw the mesh */
        void draw(Shader& shader)
        {
            for (unsigned int i = 0; i < textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                std::string name = textures[i].type;
                glUniform1i(glGetUniformLocation(shader.ID, ("material." + name).c_str()), i);
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
            }

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
  
        /* generate/bind buffers */
        void create()
        {

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(Vertex), &cubeVertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            // vertex Positions
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            glEnableVertexAttribArray(0);
            // vertex normals
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
            glEnableVertexAttribArray(1);
            // vertex texture coords
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
            glEnableVertexAttribArray(2);
            // vertex tangent      
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
            glEnableVertexAttribArray(3);
            // vertex bitangent
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
            glEnableVertexAttribArray(4);

            glBindVertexArray(0);
        }
};
#endif
