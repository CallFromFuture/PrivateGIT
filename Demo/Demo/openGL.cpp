#include "openGL.h"

bool F6 = false, F7 = true, F8 = false;
bool keys[1024];
GLfloat deltaTime = 0.0f, lastFrame = 0.0f;  // 当前帧和上一帧之间的时间差
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);
void do_movement();
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
int main() {
	//glfw init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//create window
	GLFWwindow * window = glfwCreateWindow(screenWidth, screenHeight, "learnOpenGL", nullptr, nullptr);
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
	glViewport(0, 0, screenWidth, screenHeight);
	//set callback function
	glfwSetKeyCallback(window, key_callback);
	cout << "Debug:\r\nF6: LINEAR/NEAREST\r\nF7: Z-buffer on/off\r\nF8: Fill/LINE" << endl;
	glEnable(GL_DEPTH_TEST);


	//new loader
	Shader /*shader1("vss.vert", "fss.frag"),*/shader2("vss.vert", "fss2.frag");
	Texture textureManager = *Texture::Inst();
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
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {

		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)

	};
	/*
	GLfloat vertices[] = {
		// Positions          Texture Coords
		-0.5f, 0.5f, 0.5f,  0.0f, 1.0f,//left top front 0
		0.5f,  0.5f, 0.5f,  1.0f, 1.0f,//right top front 1
		-0.5f, -0.5f, 0.5f,  0.0f, 0.0f,//left bottom front 2
		0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,//right bottom front 3
		-0.5f, 0.5f, -0.5f,  0.0f, 1.0f,//left top back 4
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,//right top back 5
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,//left bottom back 6
		0.5f,  -0.5f, -0.5f,  1.0f, 0.0f,//right bottom back 7

	};
	GLuint indices[] = {
		0,1,2,
		3,1,2,
		4,0,6,
		2,0,6,
		0,1,4,
		5,1,4,
		3,1,7,
		5,1,7,
		2,3,6,
		7,3,6,
		6,4,7,
		5,4,7

	};
	*/
	//--------------bind VAO begin---------------
	GLuint VAO, VBO, EBO;

	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);//vertices
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));//colors
	//glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));//texture coords
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	//--------------bind VAO end---------------


	GLuint transformLoc = glGetUniformLocation(shader2.Program, "transform"),
		texture1Loc = glGetUniformLocation(shader2.Program, "Texture1"),
		texture2Loc = glGetUniformLocation(shader2.Program, "Texture2");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	//mat init
	glm::mat4 model, view, projection;
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f);
	//camera
	/*
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
	*/



	

	GLint modelLoc = glGetUniformLocation(shader2.Program, "model"),
		viewLoc = glGetUniformLocation(shader2.Program, "view"),
		projectionLoc = glGetUniformLocation(shader2.Program, "projection");

	//game loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		do_movement();

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.4f, 0.8f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glActiveTexture(GL_TEXTURE0);
		textureManager.BindTexture(0);
		glUniform1i(texture1Loc, 0);
		glActiveTexture(GL_TEXTURE1);
		textureManager.BindTexture(1);
		glUniform1i(texture2Loc, 1);

		//GLfloat radius = 10.f;
		//GLfloat camX = sin(glfwGetTime()) * radius;
		//GLfloat camZ = cos(glfwGetTime()) * radius;
		//glm::mat4 view;
		//view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//shader1.use();
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		shader2.use();


		//glm::mat4 trans;
		//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		//trans = glm::rotate(trans, (GLfloat)glfwGetTime()*glm::radians(50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		//timeValue = glfwGetTime();
		//xValue = sin(timeValue);
		//glUniform1f(vertexColorLocation, xValue);
		for (GLuint i = 0; i < 10; i++) {
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			if(i==0||i==2||i==6||i==9)
				model = glm::rotate(model, angle+ (GLfloat)glfwGetTime() * glm::radians(50.f), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		//model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(50.f), glm::vec3(0.5f, 1.0f, 0.0f));

		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

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
		keys[key] = true;
		switch (key) {
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE); break;
		case GLFW_KEY_F8: 
			if (F8 == false) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 	
				F8 = true;
			} else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
				F8 = false;
			} break;
		case GLFW_KEY_F7:			
			if (F7 == false) {
				glEnable(GL_DEPTH_TEST); 
				F7 = true;
			} else {
				glDisable(GL_DEPTH_TEST); 
				F7 = false;
			} break;
		case GLFW_KEY_F6:
			if (F6 == false) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
				F6 = true;
			} else {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				F6 = false;
			} break;
		case GLFW_KEY_W:
		case GLFW_KEY_S:
		case GLFW_KEY_A:
		case GLFW_KEY_D:
			keys[key] = true; break;
		default:return;
		}

	}
	if (action == GLFW_RELEASE) {
		keys[key] = false;
	}
}

void do_movement() {
	GLfloat cameraSpeed = 5.0f * deltaTime;
	if (keys[GLFW_KEY_W])
		cameraPos += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	
}