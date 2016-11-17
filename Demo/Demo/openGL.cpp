#include "openGL.h"


void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);

int main() {
	//glfw init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//create window
	GLFWwindow * window = glfwCreateWindow(800, 600, "learnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if (window == NULL) {
		cout << "failed to create glfw window" << endl;
		glfwTerminate();
		system("PAUSE");
		return -1;
	}
	//glew init
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		cout << "Failed to initialize GLEW" << endl;
		system("PAUSE");
		return -1;
	}
	//viewport
	glViewport(0, 0, 800, 600);
	//set callback function
	glfwSetKeyCallback(window, key_callback);
	
	//---------------------load start---------------------
	/*
	const GLchar *vertexShaderSource, *fragmentShaderSource,*fragmentShaderSource2;
	GLint success;
	ifstream in1, in2,in3;
	//read vss.vert/fss.frag to vertexShaderSource/fragmentShaderSource
	in1.open("vss.vert");
	istreambuf_iterator<char> beg1(in1), end;
	string vss(beg1, end);
	vertexShaderSource = vss.c_str();
	in1.close();

	in2.open("fss.frag");
	istreambuf_iterator<char> beg2(in2);
	string fss(beg2, end);
	fragmentShaderSource = fss.c_str();
	in2.close();

	in3.open("fss2.frag");
	istreambuf_iterator<char> beg3(in3);
	string fss2(beg3, end);
	fragmentShaderSource2 = fss2.c_str();
	in3.close();

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	GLuint fragmentShader2;
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);

	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	GLuint shaderProgram,shaderProgram2;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << endl;
	}

	shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);
	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	
	if (!success) {
		glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM2::COMPILATION_FAILED\n" << infoLog << endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader2);
	*/
	//---------------------load end---------------------

	//new loader
	Shader shader1("vss.vert", "fss.frag"),shader2("vss.vert", "fss2.frag");

	GLfloat vertices[] = {
		-1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f,
		0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	GLuint indices[] = {
		0,1,2,
		0,1,3
	};
	GLuint VAO,VBO,EBO;

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	GLfloat timeValue = glfwGetTime(),xValue = sin(timeValue);
	GLint vertexColorLocation = glGetUniformLocation(shader2.Program, "deltaX");

	//game loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.4f, 0.8f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader1.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		shader2.use();

		timeValue = glfwGetTime();
		xValue = sin(timeValue);
		glUniform1f(vertexColorLocation, xValue);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)(3*sizeof(GLuint)));
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	//exit
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (key == GLFW_KEY_F8 && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (key == GLFW_KEY_F7 && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

