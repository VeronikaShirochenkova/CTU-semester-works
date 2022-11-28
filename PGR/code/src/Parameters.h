//----------------------------------------------------------------------------------------
/**
 * \file    Parameters.h
 * \author  Veronika Shirochenkova
 * \date    2022
 * \brief   Input parameters; screen size; different parameters
 */
 //----------------------------------------------------------------------------------------

// SCREEN SIZE
const unsigned int WIN_WIDTH = 1248;
const unsigned int WIN_HEIGHT = 720;


// FLAME
const unsigned int FLAME_DELAY = 5;
const unsigned int FLAME_NUM_OF_FRAMES = 35;

// LIGHT
const unsigned int POINT_LIGHT_POS = 7;								// number of point light

// BONE
glm::vec3 BONE_POSITION = glm::vec3(-0.05f, 0.60f, 0.635f);
const float BONE_SPEED = 0.02f;										// speed of bone rotation

// POTION
glm::vec3 POTION_POSITION = glm::vec3(-0.062f, 0.38f, 0.635f);		// potion position in scene
const unsigned int POTION_NUM_OF_COLORS = 4;

// FLYING CUBE
float CUBE_SPEED = 0.0001f;											// speed of flying ball

// WINDOW
glm::vec3 WINDOW_POSITION = glm::vec3(0.23f, 2.17f, -2.15f);		// position of alpha object

// COLLISION
float X_p =  2.20f;		// right wall
float X_n = -2.20f;		// left wall

float Y_p = 3.35f;		// celling
float Y_n = 0.2f;		// floor

float Z_p = 1.80f;		// front wall
float Z_n = -1.90f;		// back wall
