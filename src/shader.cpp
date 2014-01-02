#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "shader.h"


ShaderProgram::ShaderProgram(const char *vertShaderPath, const char *fragShaderPath) :
    programId(0),
    vertexShaderPath(vertShaderPath),
    fragmentShaderPath(fragShaderPath)
{
}

 ShaderProgram::~ShaderProgram()
 {
 }

void ShaderProgram::load()
{
    // Create the shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(this->vertexShaderPath.c_str(), std::ios::in);
	if (vertexShaderStream.is_open()) {
		std::string line = "";
		while(getline(vertexShaderStream, line))
			vertexShaderCode += "\n" + line;
		vertexShaderStream.close();
	} else {
		printf("Impossible to open %s. Are you in the right directory ?\n", this->vertexShaderPath.c_str());
        throw runtime_error(__func__ + string("() failed when loading file ") + this->vertexShaderPath);
	}

	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(this->fragmentShaderPath.c_str(), std::ios::in);
	if(fragmentShaderStream.is_open()){
		std::string line = "";
		while(getline(fragmentShaderStream, line))
			fragmentShaderCode += "\n" + line;
		fragmentShaderStream.close();
	} else {
		printf("Impossible to open %s. Are you in the right directory ?\n", this->fragmentShaderPath.c_str());
		throw runtime_error(__func__ + string("() failed when loading file ") + this->fragmentShaderPath);
	}

	GLint result = GL_FALSE;
	int infoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", this->vertexShaderPath.c_str());
	char const * vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer , NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		std::vector<char> vertexShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", this->fragmentShaderPath.c_str());
	char const * fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer , NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		std::vector<char> fragmentShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		printf("%s\n", &fragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	this->programId = glCreateProgram();
	glAttachShader(this->programId, vertexShaderID);
	glAttachShader(this->programId, fragmentShaderID);
	glLinkProgram(this->programId);

	// Check the program
	glGetProgramiv(this->programId, GL_LINK_STATUS, &result);
	glGetProgramiv(this->programId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		std::vector<char> programErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(this->programId, infoLogLength, NULL, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

GLint ShaderProgram::attrib(const GLchar *attribName) const
{
    if (this->programId == 0) {
        throw runtime_error(__func__ + string("(): shader is not loaded: ") + this->vertexShaderPath + string(", ") + this->fragmentShaderPath);
    }
    if(!attribName) {
        throw runtime_error( __func__ + string("(): attribName was NULL"));
    }
    
    GLint attrib = glGetAttribLocation(this->programId, attribName);
    if(attrib == -1) {
        cerr << "Shader attribute not found: " << attribName << std::endl;
        throw runtime_error(std::string("Shader attribute not found: ") + attribName);
    }
    
    return attrib;
}

GLuint ShaderProgram::id() const
{
    return this->programId;
}

void ShaderProgram::use() const 
{
    glUseProgram(this->programId);
}

void ShaderProgram::stopUsing() const 
{
    glUseProgram(0);
}