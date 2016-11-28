#include "Shader.h"





Shader::Shader(const GLchar * vertexSourcePath, const GLchar * fragmentSourcePath) {
	const GLchar *vertexShaderSource, *fragmentShaderSource;

	try {
		ifstream in1(vertexSourcePath), in2(fragmentSourcePath);

		istreambuf_iterator<char> beg1(in1),beg2(in2), end;

		//string vss(beg1, end) , fss(beg2, end);
		vertexShaderSource = ((new string(beg1, end))->c_str());
		fragmentShaderSource = ((new string(beg2, end))->c_str());
		in1.close();
		in2.close();
		//cout << vertexShaderSource << fragmentShaderSource << endl;
	}
	catch(exception e){
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << e.what() << endl;
	}
	GLint success;
	GLuint vertexShader, fragmentShader;
	GLchar infoLog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	Program = glCreateProgram();
	glAttachShader(Program, vertexShader);
	glAttachShader(Program, fragmentShader);
	glLinkProgram(Program);

	glGetProgramiv(Program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(Program, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	delete vertexShaderSource;
	delete fragmentShaderSource;
}


Shader::~Shader() {
}

void Shader::use() {
	glUseProgram(Program);
}
