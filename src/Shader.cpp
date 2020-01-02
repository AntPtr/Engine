#include "Shader.h"

cShaderProg::cShaderProg(const std::string vertexPath, const std::string fragPath)
{
	shaderInt(vertexPath, fragPath);
}

void cShaderProg::shaderInt(std::string vertx, std::string frag) {
	//Vertex File reading 
	std::ifstream ver_file(vertx);
	std::stringstream vertx_ss;
	std::string mVertex;
	const char* vertex_source;

	if (ver_file) {
		vertx_ss << ver_file.rdbuf();
		mVertex = vertx_ss.str();
		vertex_source = mVertex.c_str();
	}
	else
		std::cerr << "Couldn't open Vertex's File" << std::endl;
	
	//Fragment File reading 
	std::ifstream frag_file(frag);
	std::stringstream frag_ss;
	std::string mFrag;
	const char* frag_source;

	if (frag_file) {
		frag_ss << frag_file.rdbuf();
		mFrag = frag_ss.str();
		frag_source = mFrag.c_str();
	}
	else
		std::cerr << "Couldn't open Fragment's File" << std::endl;
	
	//Create Vertex Shader  
	unsigned int vertexShader{ 0 };
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex_source, NULL);

	glCompileShader(vertexShader);

	int success{ 0 };
	char info[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		std::cout << "FAILED TO COMPILE VERTEX SHADER: " << info << std::endl;
	}

	//Create Fragment Shader
	unsigned int fragShader{ 0 };
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &frag_source, NULL);

	glCompileShader(fragShader);

	success = 0;
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		std::cout << "FAILED TO COMPILE VERTEX SHADER: " << info << std::endl;
	}

	//Crate Program Shader
	mID = glCreateProgram();
	glAttachShader(mID, vertexShader);
	glAttachShader(mID, fragShader);

	glLinkProgram(mID);

	success = 0;
	glGetProgramiv(mID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(mID, 512, NULL, info);
		std::cout << "Error in linking program :" << info << std::endl;
	}

	glDeleteShader(fragShader);
	glDeleteShader(vertexShader);

}

void cShaderProg::use() {
	glUseProgram(mID);
}

int cShaderProg::get_id() {
	return mID;
}

void cShaderProg::set_bool(const std::string& name, bool value) {
	glUniform1i(glGetUniformLocation(mID, name.c_str()), (int)(value));
}

void cShaderProg::set_int(const std::string& name, int value) {
	glUniform1i(glGetUniformLocation(mID, name.c_str()), value);
}

void cShaderProg::set_float(const std::string& name, float value) {
	glUniform1f(glGetUniformLocation(mID, name.c_str()), value);
}