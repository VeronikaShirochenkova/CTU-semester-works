//----------------------------------------------------------------------------------------
/**
 * \file    Model.h
 * \author  Veronika Shirochenkova
 * \date    2022
 * \brief   Model class implementation; Functions for loading models; Assimp
 */
 //----------------------------------------------------------------------------------------

#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include<stb/stb_image.h>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

unsigned int textureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
    public:
        // model data 
        std::vector<Texture> loadedTextures;
        std::vector<Mesh> meshes;
        std::string directory;


        /* Constructor */
        Model(std::string const& path)
        {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
            // check for errors
            if (!scene || !scene->mRootNode) {
                std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
                return;
            }

            directory = path.substr(0, path.find_last_of('/'));

            processNode(scene->mRootNode, scene);
        }

        /* call mesh draw function */
        void draw(Shader& shader)
        {
            for (unsigned int i = 0; i < meshes.size(); i++)
                meshes[i].draw(shader);
        }

        /* process scene nodes */
        void processNode(aiNode* node, const aiScene* scene)
        {
            for (unsigned int i = 0; i < node->mNumMeshes; i++) {

                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(createMesh(mesh, scene));
            }
            for (unsigned int i = 0; i < node->mNumChildren; i++) {
                processNode(node->mChildren[i], scene);
            }

        }

        Mesh createMesh(aiMesh* mesh, const aiScene* scene)
        {

            std::vector<Vertex> cubeVertices;
            std::vector<unsigned int> indices;
            std::vector<Texture> textures;

            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                glm::vec3 vector; 
                // positions
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.Position = vector;
                // normals
                if (mesh->HasNormals())
                {
                    vector.x = mesh->mNormals[i].x;
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.Normal = vector;
                }
                // texture coordinates
                if (mesh->mTextureCoords[0])
                {
                    glm::vec2 vec;

                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.TexCoords = vec;
                    // tangent
                    vector.x = mesh->mTangents[i].x;
                    vector.y = mesh->mTangents[i].y;
                    vector.z = mesh->mTangents[i].z;
                    vertex.Tangent = vector;
                    // bitangent
                    vector.x = mesh->mBitangents[i].x;
                    vector.y = mesh->mBitangents[i].y;
                    vector.z = mesh->mBitangents[i].z;
                    vertex.Bitangent = vector;
                }
                else
                    vertex.TexCoords = glm::vec2(0.0f, 0.0f);

                cubeVertices.push_back(vertex);
            }
        
            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
            
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }

            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            // 1. diffuse maps
            std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. specular maps
            std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            // 3. normal maps
            std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

            return Mesh(cubeVertices, indices, textures);
        }


        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
        {
            std::vector<Texture> textures;
            int num_of_textures = mat->GetTextureCount(type);
            for (int i = 0; i < num_of_textures; i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);

                bool skip = false;
                for (unsigned int j = 0; j < loadedTextures.size(); j++)
                {
                    if (std::strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0)
                    {
                        textures.push_back(loadedTextures[j]);
                        skip = true;
                        break;
                    }
                }
                if (!skip)
                {   
                    Texture texture;
                    texture.id = textureFromFile(str.C_Str(), this->directory);
                    texture.type = typeName;
                    texture.path = str.C_Str();
                    textures.push_back(texture);
                    loadedTextures.push_back(texture);
                }
            }
            return textures;
        }
};

/* read texture from file */
unsigned int textureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, channels;
    
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    
    if (data)
    {
        GLenum format;
        if (channels == 1)
            format = GL_RED;
        else if (channels == 3)
            format = GL_RGB;
        else if (channels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load model texture at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


void loadModels(std::vector<Model>* Models) 
{
    // 1st list
    Model floor("Models/WitchRoom/1st_list/floor.obj");
    Models->push_back(floor);

    Model columns("Models/WitchRoom/1st_list/columns.obj");
    Models->push_back(columns);

    Model magic_table("Models/WitchRoom/1st_list/magic_table.obj");
    Models->push_back(magic_table);

    
    
    // 2nd list
    Model fireplace("Models/WitchRoom/2nd_list/fireplace.obj");
    Models->push_back(fireplace);

    Model door("Models/WitchRoom/2nd_list/door.obj");
    Models->push_back(door);

    
    
    // 3rd list
    Model cupboard("Models/WitchRoom/3rd_list/cupboard.obj");
    Models->push_back(cupboard);

    Model table("Models/WitchRoom/3rd_list/table.obj");
    Models->push_back(table);

    Model shelf("Models/WitchRoom/3rd_list/shelf.obj");
    Models->push_back(shelf);

    Model chests("Models/WitchRoom/3rd_list/chests.obj");
    Models->push_back(chests);

    Model stairs("Models/WitchRoom/3rd_list/stairs.obj");
    Models->push_back(stairs);

    
    
    // 4th list
    Model books("Models/WitchRoom/4th_list/books.obj");
    Models->push_back(books);

    Model candles("Models/WitchRoom/4th_list/candles.obj");
    Models->push_back(candles);

    Model bones("Models/WitchRoom/4th_list/bones.obj");
    Models->push_back(bones);

    Model skulls("Models/WitchRoom/4th_list/skulls.obj");
    Models->push_back(skulls);

    Model flasks("Models/WitchRoom/4th_list/flasks.obj");
    Models->push_back(flasks);

    Model mushrooms("Models/WitchRoom/4th_list/mushrooms.obj");
    Models->push_back(mushrooms);

    Model horns("Models/WitchRoom/4th_list/horns.obj");
    Models->push_back(horns);

    Model crystals("Models/WitchRoom/4th_list/crystals.obj");
    Models->push_back(crystals);

    Model magic_ball("Models/WitchRoom/4th_list/magic_ball.obj");
    Models->push_back(magic_ball);

    Model magic_ball_base("Models/WitchRoom/4th_list/magic_ball_base.obj");
    Models->push_back(magic_ball_base);

    Model table_cloth("Models/WitchRoom/4th_list/table_cloth.obj");
    Models->push_back(table_cloth);

    Model main_kettle("Models/WitchRoom/4th_list/main_kettle.obj");
    Models->push_back(main_kettle);

    Model kettles("Models/WitchRoom/4th_list/kettles.obj");
    Models->push_back(kettles);

    Model papers("Models/WitchRoom/4th_list/papers.obj");
    Models->push_back(papers);

    Model stacks_of_books("Models/WitchRoom/4th_list/stacks_of_books.obj");
    Models->push_back(stacks_of_books);

    
    
    // 5th list
    Model walls("Models/WitchRoom/5th_list/walls_w.obj");
    Models->push_back(walls);

    Model window_frame("Models/WitchRoom/5th_list/window_frame.obj");
    Models->push_back(window_frame);

    Model window_partition("Models/WitchRoom/5th_list/window_partition.obj");
    Models->push_back(window_partition);

    Model windowsill("Models/WitchRoom/5th_list/windowsill.obj");
    Models->push_back(windowsill);

    Model trapdoor("Models/WitchRoom/5th_list/trapdoor.obj");
    Models->push_back(trapdoor);

    Model trapdoor_handle_part("Models/WitchRoom/5th_list/trapdoor_handle_part.obj");
    Models->push_back(trapdoor_handle_part);

    Model trapdoor_handle("Models/WitchRoom/5th_list/trapdoor_handle.obj");
    Models->push_back(trapdoor_handle);

    Model celling_cloth("Models/WitchRoom/5th_list/celling_cloth.obj");
    Models->push_back(celling_cloth);
}
#endif
