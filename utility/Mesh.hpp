#ifndef MESH_HPP
#define MESH_HPP

#include "Ein.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/types.h>

struct Vertex {
    // Position
    glm::vec3 Position;
    // Normal
    glm::vec3 Normal;
    // TexCoords
    glm::vec2 TexCoords;
    // Tangent
    glm::vec3 Tangent;
    // Bitangent
    glm::vec3 Bitangent;
};

class Mesh {
public:
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;
    GLuint VAO;

    /*  Functions  */
    // Constructor
    Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);

    // Render the mesh
    void Draw(Shader shader);

private:
    /*  Render data  */
    GLuint VBO;
    GLuint EBO;
};

#endif
