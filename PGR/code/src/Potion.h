//----------------------------------------------------------------------------------------
/**
 * \file    Potion.h
 * \author  Veronika Shirochenkova
 * \date    2022
 * \brief   Potion class implementation; Potion dynamic texture for main kettle
 */
 //----------------------------------------------------------------------------------------

#ifndef POTION_H
#define POTION_H

#include <glm/glm/glm.hpp>

const float potionVertices[] = 
{
     0.0f,   0.0f,   0.0f,         1.0f,   1.0f,    // 0
     1.0f,   0.0f,   0.0f,         2.0f,   1.0f,    // 1
     0.75f,  0.75f,  0.0f,         1.75f,  1.75f,   // 2
     0.0f,   1.0f,   0.0f,         1.0f,   2.0f,    // 3
    -0.75f,  0.75f,  0.0f,         0.25f,  1.75f,   // 4
    -1.0f,   0.0f,   0.0f,         0.0f,   1.0f,    // 5
    -0.75f, -0.75f,  0.0f,         0.25f,  0.25f,   // 6
     0.0f,  -1.0f,   0.0f,         1.0f,   0.0f,    // 7
     0.75f, -0.75f,  0.0f,         1.75f,  0.25f,   // 8
};


const unsigned int potionIndices[] =
{   
    0, 1, 2,
    0, 2, 3,
    0, 3, 4,
    0, 4, 5,
    0, 5, 6,
    0, 6, 7,
    0, 7, 8,
    0, 8, 1
};


glm::vec4 potionColor[] = {
    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  // red
    glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),  // blue
    glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  // green
    glm::vec4(0.5f, 0.0f, 0.5f, 1.0f)   // purple
};

const std::string potionColors[] = 
{
    "red", "blue", "green", "purple"
};

class Potion {
    public:
        // buffers
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;

        glm::vec3 position;

        unsigned int texture;
        unsigned int color;

        /* Constructor */
        Potion(glm::vec3 pos) 
        {
            position.x = pos.x;
            position.y = pos.y;
            position.z = pos.z;
            color = 3;
        }

        /* generate/bind buffers */
        void create() 
        {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(potionVertices), potionVertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(potionIndices), potionIndices, GL_STATIC_DRAW);


            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);


            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        /* load texture */
        void loadTexture() 
        {
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            int width, height, channels;
            std::string path = "Potion/Potion.png";

            stbi_set_flip_vertically_on_load(true);
            unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                std::cout << "Failed to load potion texture" << std::endl;
            }
            stbi_image_free(data);
            stbi_set_flip_vertically_on_load(false);


            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        /* change potion color */
        void changeColor() 
        {
            color++;
            if (color == POTION_NUM_OF_COLORS) {
                color = 0;
            }
        }
};
#endif
