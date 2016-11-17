#pragma once
#include <string>
#include <fstream>
#include <iostream>
using namespace std;
#include <GL/glew.h>
class Shader
{
public:
	GLuint Program;
	Shader(const GLchar * vertexSourcePath,const GLchar * fragmentSourcePath);
	~Shader();
	void use();
};

