//----------------------------------------------------------------------------------------
/**
 * \file    Cube.h
 * \author  Veronika Shirochenkova
 * \date    2022
 * \brief   Cube class implementation; Flying cube that simply follows a Bezier curve
 */
 //----------------------------------------------------------------------------------------

#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>


float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

glm::vec3 curvePoints[] = {
    glm::vec3(-2.11f,  2.60f,   2.00f),
    glm::vec3(-1.90f,  0.35f,  -8.92f),
    glm::vec3(1.90f,  3.09f,   8.76f),
    glm::vec3(1.90f,  2.39f,  -2.00f)
};

class Cube 
{
    public:
        // buffers
        unsigned int VAO;
        unsigned int VBO;

        glm::vec3 position;

        // animation parameters
        float animationTime;
        float speed;
        float rotation;

        /* Constructor */
        Cube(float sp) 
        {
            position = curvePoints[0];
            animationTime = 0;
            speed = sp;
            rotation = 0.0f;
        }

        /* generate/bind buffers */
        void create() 
        {
            glGenBuffers(1, &VBO);
            glGenVertexArrays(1, &VAO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glBindVertexArray(0);
        }

        void time() 
        {
            animationTime += speed;
            if (animationTime > 1) {
                animationTime = 1;
                speed *= -1;
            }
            else if (animationTime < 0) {
                animationTime = 0;
                speed *= -1;
            }
        }

        /* calculate position depenging on time/curve points */
        void ñurveAnimation()
        {
            /* 
            *   Bezier curve with 4 control points
                B(t) = P1 * ( 1 — t )^3 + P2 * 3 * t * ( 1 — t )^2 + P3 * 3 * t^2 * ( 1 — t ) + P4 * t^3 
            */

            /* (1-t)    */
            float var1 = 1 - animationTime;
            /* (1-t)^3  */
            float var2 = var1 * var1 * var1;
            /* t^3      */
            float var3 = animationTime * animationTime * animationTime;

            position.x = var2 * curvePoints[0].x + 3 * animationTime * var1 * var1 * curvePoints[1].x + 3 * animationTime * animationTime * var1 * curvePoints[2].x + var3 * curvePoints[3].x;
            position.y = var2 * curvePoints[0].y + 3 * animationTime * var1 * var1 * curvePoints[1].y + 3 * animationTime * animationTime * var1 * curvePoints[2].y + var3 * curvePoints[3].y;
            position.z = var2 * curvePoints[0].z + 3 * animationTime * var1 * var1 * curvePoints[1].z + 3 * animationTime * animationTime * var1 * curvePoints[2].z + var3 * curvePoints[3].z;
            rotation += 0.2f;
            if (rotation > 360.0f) {
                rotation = 0.0f;
            }
        }
};
#endif
