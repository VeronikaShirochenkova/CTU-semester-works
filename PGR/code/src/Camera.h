//----------------------------------------------------------------------------------------
/**
 * \file    Camera.h
 * \author  Veronika Shirochenkova
 * \date    2022
 * \brief   Camera class implementation
 */
 //----------------------------------------------------------------------------------------

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

// Camera parameters
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 80.0f;

// Static view parameters
glm::vec3 staticViewPos[] = {
    glm::vec3(-2.110f, 3.331f,  1.666f),
    glm::vec3( 2.200f, 2.930f, -1.800f)
};
GLfloat staticViewAngles[2][2] = {
    {-40.00f, -36.0f},
    {-216.4f, -29.5f}
};

class Camera
{
    public:
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        float Yaw;
        float Pitch;

        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

        bool firstMouseUse;
        bool staticView;


        bool StaticView1;
        bool StaticView2;

        /*Constructor*/
        Camera(glm::vec3 position)
        {
            Position = position;
            Front = glm::vec3(0.0f, 0.0f, -1.0f);
            WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

            Yaw = YAW;
            Pitch = PITCH;
        
            MovementSpeed = SPEED;
            MouseSensitivity = SENSITIVITY;
            Zoom = ZOOM;

            firstMouseUse = true;
            staticView = true;


            StaticView1 = false;
            StaticView2 = false;

            updateCameraVectors();
        }

        /* change camera position depending on the time / collision */
        void processKeyboard(Camera_Movement direction, float deltaTime, bool collision)
        {

            float velocity = MovementSpeed * deltaTime;

            if (!collision) {
                if (direction == FORWARD)         
                    Position += Front * velocity;
                if (direction == BACKWARD)
                    Position -= Front * velocity;
                if (direction == LEFT)
                    Position -= Right * velocity;
                if (direction == RIGHT)
                    Position += Right * velocity;
            }
            else {
                glm::vec3 point = glm::vec3(-0.05f, 0.4f, 0.62f);
                glm::vec3 p = glm::vec3(0.0f);

                if (direction == FORWARD)
                    p = Position + Front * velocity;
                if (direction == BACKWARD)
                    p = Position - Front * velocity;
                if (direction == LEFT)
                    p = Position - Right * velocity;
                if (direction == RIGHT)
                    p = Position + Right * velocity;

                float x = p.x - point.x;
                float y = p.y - point.y;
                float z = p.z - point.z;
                float len = sqrt(x * x + y * y + z * z);

                if (len > 0.5f) {
                    if (p.x > X_n && p.x < X_p &&
                        p.y > Y_n && p.y < Y_p &&
                        p.z > Z_n && p.z < Z_p) 
                    {
                        Position = p;
                    }
                }
            }
        }

        /*rotate camera*/
        void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
        {
            xoffset *= MouseSensitivity;
            yoffset *= MouseSensitivity;

            Yaw += xoffset;
            Pitch += yoffset;

            if (constrainPitch)
            {
                if (Pitch > 89.0f)
                    Pitch = 89.0f;
                if (Pitch < -89.0f)
                    Pitch = -89.0f;
            }

            updateCameraVectors();
        }


        void updateCameraVectors()
        {
            glm::vec3 front;
            front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            front.y = sin(glm::radians(Pitch));
            front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            Front = glm::normalize(front);

            Right = glm::normalize(glm::cross(Front, WorldUp));  
            Up = glm::normalize(glm::cross(Right, Front));
        }


        /* setup start camera rotation */
        void setupStartView() {
            Yaw = staticViewAngles[0][0];
            Pitch = staticViewAngles[0][1];
            updateCameraVectors();
        }

};
#endif
