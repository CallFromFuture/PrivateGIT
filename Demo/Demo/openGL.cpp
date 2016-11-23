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
	
	

	//new loader
	Shader /*shader1("vss.vert", "fss.frag"),*/shader2("vss.vert", "fss2.frag");
	Texture textureManager= *Texture::Inst();
	if (!textureManager.loadTexture("test1.jpg", 0, GL_BGR, GL_RGB, 0, 0)) {
		cout << "ERROR::TEXTURE::LAODER::LAODING_FAILED\n" << endl;
		system("PAUSE");
		return -1;
	}
	if (!textureManager.loadTexture("huaji.png", 1, GL_BGRA, GL_RGB, 0, 0)) {
		cout << "ERROR::TEXTURE::LAODER::LAODING_FAILED\n" << endl;
		system("PAUSE");
		return -1;
	}
	GLfloat vertices[] = {
		// Positions // Colors // Texture Coords
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // Top Left
	};
	GLuint indices[] = {
		0,1,3,
		1,2,3
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);//vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));//colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));//texture coords
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	GLuint transformLoc = glGetUniformLocation(shader2.Program, "transform"),
		texture1Loc= glGetUniformLocation(shader2.Program, "Texture1"),
		texture2Loc = glGetUniformLocation(shader2.Program, "Texture2");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	//game loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.4f, 0.8f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		textureManager.BindTexture(0);
		glUniform1i(texture1Loc, 0);
		glActiveTexture(GL_TEXTURE1);
		textureManager.BindTexture(1);
		glUniform1i(texture2Loc, 1);


		//shader1.use();
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		shader2.use();


		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (GLfloat)glfwGetTime()*glm::radians(50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		//timeValue = glfwGetTime();
		//xValue = sin(timeValue);
		//glUniform1f(vertexColorLocation, xValue);
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)(3*sizeof(GLuint)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glEnableVertexAttribArray(0);

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
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE); break;
		case GLFW_KEY_F8: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
		case GLFW_KEY_F7: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
		case GLFW_KEY_F6:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case GLFW_KEY_F5:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		default:return;
		}

	}
}

