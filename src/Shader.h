#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class cShaderProg
{
	private :
		int mID;

	public :
		cShaderProg(const std::string vertexPath, const std::string fragPath);
		void shaderInt(std::string , std::string);
		void use();
		int get_id();
		void set_bool(const std::string &name, bool value);
		void set_int(const std::string &name, int value);
		void set_float(const std::string& name, float value);
};

