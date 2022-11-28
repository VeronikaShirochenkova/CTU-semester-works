//----------------------------------------------------------------------------------------
/**
 * \file    Glass.h
 * \author  Veronika Shirochenkova
 * \date    2022
 * \brief   Glass class implementation; Window glass
 */
 //----------------------------------------------------------------------------------------

#ifndef GLASS_H
#define GLASS_H


float glassVertices[] = 
{
    -0.5f, -0.5f, 0.0f,    0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,    1.0f, 0.0f,
     0.5f,  0.5f, 0.0f,    1.0f, 1.0f,
    -0.5f,  0.5f, 0.0f,    0.0f, 1.0f 
};

unsigned int glassIndices[] = 
{ 
    0, 1, 2,
    2, 3, 0 
};

class Glass
{
    public:
        // buffers
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;

        Glass() 
        {

        }

        /* generate/bind buffers */
        void create() 
        {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glassVertices), glassVertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glassIndices), glassIndices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

};
#endif
