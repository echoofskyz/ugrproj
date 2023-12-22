#include <stdio.h>
#include "ShaderUtils.h"

unsigned int compileFromChars(const char* vertShaderSource, const char* fragShaderSource) {
	int success;
	char infoLog[512];

	unsigned int vertShader;
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertShaderSource, NULL);
	glCompileShader(vertShader);
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	
	if (!success) {
		glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
	}

	unsigned int fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragShaderSource, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	
	if (!success) {
		glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
	}
	
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	
	if(!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINK_FAILED\n%s\n", infoLog);
	}

	// clean up unneeded shaders
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	
	return shaderProgram;
}

const struct ShaderUtils ShaderUtils = {
	.compileFromChars = compileFromChars
};