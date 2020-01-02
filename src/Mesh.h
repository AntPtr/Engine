#pragma once
#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>


struct Vertex {
	glm::vec3 position;
	glm::vec3 normals;
	glm::vec2 coords;
};


struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	unsigned int VAO;
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;
	Mesh(std::vector<Vertex> vert, std::vector<Texture> tex, std::vector<unsigned int> ind);
	void draw(cShaderProg shader);
private:
	unsigned int VBO, EBO;
	void setupMesh();
};

