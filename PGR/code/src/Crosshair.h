//----------------------------------------------------------------------------------------
/**
 * \file    Crosshair.h
 * \author  Veronika Shirochenkova
 * \date    2022
 * \brief   Crosshair class implementation; Crosshair is HUD part
 */
 //----------------------------------------------------------------------------------------

#ifndef CROSSHAIR_H
#define CROSSHAIR_H

float crosshairVertices[] =
{
        -1.0f, -1.0f,   0.0f,         0.0f,    0.0f,
         1.0f, -1.0f,   0.0f,         1.0f,    0.0f,
         1.0f,  1.0f,   0.0f,         1.0f,    1.0f,
        -1.0f,  1.0f,   0.0f,         0.0f,    1.0f
};

unsigned int crosshairIndices[] =
{
    0, 1, 2,
    2, 3, 0
};


class Crosshair
{
public:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int texture;

    Crosshair() { }

    /* generate/bind buffers */
    void create()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(crosshairIndices), crosshairIndices, GL_STATIC_DRAW);


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
        std::string path = "Crosshair/crosshair.png";

        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load crosshair texture" << std::endl;
        }
        stbi_image_free(data);
        stbi_set_flip_vertically_on_load(false);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
};
#endif
