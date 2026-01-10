#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "../../thirdparty/stb_image/stb_image.h"
#include "../../thirdparty/glad/glad.h"
#include "Mesh.h"
#include "Shader.h"

using namespace std;

static unsigned int TextureFromFile(const std::string &filename, const std::string &directory, const aiScene &scene) {
    int width, height, channels;
    GLenum format;
    unsigned char *data;

    data = stbi_load((directory + '/' + filename).c_str(),
                     &width,
                     &height,
                     &channels,
                     0);
    if (!data) {
        const aiTexture *tex = scene.GetEmbeddedTexture(filename.c_str());
        if (tex->mHeight == 0) {
            // Compressed (PNG/JPG)
            data = stbi_load_from_memory(
                reinterpret_cast<const unsigned char *>(tex->pcData),
                tex->mWidth,
                &width, &height, &channels, 0 // force RGBA
            );
        } else {
            data = reinterpret_cast<unsigned char *>(tex->pcData);
            width = tex->mWidth;
            height = tex->mHeight;
            format = GL_RGBA;
            // TODO check for format type by tex->achFormatHint
        }
    }

    if (!data) {
        std::cerr << "Texture failed to load at path: " << filename << std::endl;
        std::cerr << "Failed to decode embedded texture\n";
        return 0;
    }


    if (channels == 1) {
        format = GL_RED;
    } else if (channels == 3) {
        format = GL_RGB;
    } else {
        format = GL_RGBA;
    }

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return textureID;
}

class Model {
public:
    // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Texture> textures_loaded;
    vector<Mesh> meshes;
    glm::vec3 max_vertex;
    glm::vec3 min_vertex;
    string directory;
    bool gammaCorrection;

    // constructor, expects a filepath to a 3D model.
    explicit Model(string const &path, bool gamma = false)
        : max_vertex(glm::vec3(std::numeric_limits<float>::lowest())),
          min_vertex(glm::vec3(std::numeric_limits<float>::max())),
          gammaCorrection(gamma) {
        loadModel(path);
    }

    // draws the model, and thus all its meshes
    void Draw(Shader &shader) {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

private:
    void processAABB(glm::vec3 const &vert) {
        max_vertex.x = std::max(max_vertex.x, vert.x);
        max_vertex.y = std::max(max_vertex.y, vert.y);
        max_vertex.z = std::max(max_vertex.z, vert.z);

        min_vertex.x = std::min(min_vertex.x, vert.x);
        min_vertex.y = std::min(min_vertex.y, vert.y);
        min_vertex.z = std::min(min_vertex.z, vert.z);
    }

    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const &path) {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(
            path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }

        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, *scene);
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene &scene) {
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            // the node object only contains indices to index the actual objects in the scene.
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh &mesh = *scene.mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh &mesh, const aiScene &scene) {
        // data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
            Vertex vertex;
            glm::vec3 vector;
            // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh.mVertices[i].x;
            vector.y = mesh.mVertices[i].y;
            vector.z = mesh.mVertices[i].z;
            vertex.Position = vector;
            processAABB(vertex.Position);
            // normals
            if (mesh.HasNormals()) {
                vector.x = mesh.mNormals[i].x;
                vector.y = mesh.mNormals[i].y;
                vector.z = mesh.mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if (mesh.mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh.mTextureCoords[0][i].x;
                vec.y = mesh.mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = mesh.mTangents[i].x;
                vector.y = mesh.mTangents[i].y;
                vector.z = mesh.mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh.mBitangents[i].x;
                vector.y = mesh.mBitangents[i].y;
                vector.z = mesh.mBitangents[i].z;
                vertex.Bitangent = vector;
            } else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        for (unsigned int i = 0; i < mesh.mNumFaces; i++) {
            aiFace face = mesh.mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial *material = scene.mMaterials[mesh.mMaterialIndex];

        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<Texture> specularMaps =
                loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", scene);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", scene);
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        std::vector<Texture> heightMaps =
                loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", scene);
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        return Mesh(vertices, indices, textures);
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName, const aiScene &scene) {
        vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++) {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip) {
                // if texture hasn't been loaded already, load it
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory, scene);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(std::move(texture));
                textures_loaded.push_back(textures.back());
                // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
            }
        }
        return textures;
    }
};


#endif //MODEL_H
