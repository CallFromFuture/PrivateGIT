#include "openGL.h"
static GLuint axes_list;
bool F6 = false, F7 = true, F8 = false,firstMouse = true;
bool keys[1024];
GLfloat deltaTime = 0.0f, lastFrame = 0.0f;
HANDLE hStdout;

int main();
//GLfloat yaw = -90.0f, pitch = 0.0f;
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void do_movement();
void CalculateFrameRate();
void drawAxisInit();
void outputDebugInfo();
COORD moveCursor(int row, int column);

void outputVec3(glm::vec3 vec);

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);

bool useResource(int id, char * type, char * filename);

Camera camera = *Camera::Inst();
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	cout << "Debugging:" << endl;//0
	cout << "F5: UNDEFINED" << endl;//1
	cout << "F6: ReturnToOrigin" << endl;//2
	cout << "F7: Z-buffer on/off" << endl;//3
	cout << "F8: Fill/LINE" << endl;//4
	cout << "fps:  " << endl;//5
	cout << "pos:  " << endl;//6
	cout << "view: " << endl;//7
	cout << "up:   " << endl;//8
	cout << "x1:   " << endl;//9
	cout << "y1:   " << endl;//10
	cout << "z1:   " << endl;//11
	cout << "zoom: " << endl;//12
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO CCI;
	CCI.bVisible = false;
	CCI.dwSize = sizeof(CCI);
	SetConsoleCursorInfo(hStdout, &CCI);
	glEnable(GL_DEPTH_TEST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#ifndef _DEBUG
	CreateDirectory("res", NULL);
	useResource(IDR_JPG1, "JPG", "res/test1.jpg");
	useResource(IDR_JPG2, "JPG", "res/huaji.jpg");
	useResource(IDR_SHADER1, "shader", "res/fss2.frag");
	useResource(IDR_SHADER2, "shader", "res/vss.vert");
#else
	cout << "DEBUG mode" << endl;
#endif
	//vsync on/off
	/*
	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

	wglSwapIntervalEXT(1);//on
	wglSwapIntervalEXT(0);//off
	*/

	//drawAxisInit();
	cout << glGetString(GL_RENDERER);
	//new loader
	Shader	lightingShader("res/vss.vert", "res/lightingFss.frag"),
			shader2("res/vss.vert", "res/fss2.frag"),
			lightShader("res/vss.vert", "res/lightFss.frag");

	Texture textureManager = *Texture::Inst();
	/*
	if (!textureManager.loadTexture("JPG.jpg", NUM, GL_BGR, GL_RGB, 0, 0)) {//JPG FORMAT
		cout << "ERROR::TEXTURE::LAODER::LAODING_FAILED\n" << endl;
		system("PAUSE");
		return -1;
	}
	if (!textureManager.loadTexture("PNG.png", NUM, GL_BGRA, GL_RGB, 0, 0)) {//PNG FORMAT
		cout << "ERROR::TEXTURE::LAODER::LAODING_FAILED\n" << endl;
		system("PAUSE");
		return -1;
	}*/
	if (!textureManager.loadTexture("res/container2.png", 0, GL_BGRA, GL_RGB, 0, 0)) {//PNG FORMAT
		cout << "ERROR::TEXTURE::LAODER::LAODING_FAILED\n" << endl;
		system("PAUSE");
		return -1;
	}
	
	if (!textureManager.loadTexture("res/container2_specular.png", 1, GL_BGRA, GL_RGB, 0, 0)) {//PNG FORMAT
		cout << "ERROR::TEXTURE::LAODER::LAODING_FAILED\n" << endl;
		system("PAUSE");
		return -1;
	}

	if (!textureManager.loadTexture("res/matrix.jpg", 2, GL_BGR, GL_RGB, 0, 0)) {//JPG FORMAT
		cout << "ERROR::TEXTURE::LAODER::LAODING_FAILED\n" << endl;
		system("PAUSE");
		return -1;
	}
	GLfloat vertices[] = {
		// Positions           // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
		
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

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
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
	//generate
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	

	//bind VAO
	glBindVertexArray(VAO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);//vertices
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));//colors
	//glEnableVertexAttribArray(1);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));//normal
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));//texture coords
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	//end

	//bind lightVAO
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	//end

	//--------------bind VAO end---------------


	//mat init

	glm::mat4 /*model,*/ view, projection,emptyModel;
	//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(glm::radians(camera.info.zoom), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);


	//light init
	glm::mat4 lightModel;
	glm::vec3 objectColor/*temp*/, lightColor;
	objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);//white
	lightModel = glm::scale(glm::translate(lightModel, lightPos), glm::vec3(0.2f));

	lightingShader.use();
	GLint	lightingModelLoc = glGetUniformLocation(lightingShader.Program, "model"),
		lightingViewLoc = glGetUniformLocation(lightingShader.Program, "view"),
		lightingProjectionLoc = glGetUniformLocation(lightingShader.Program, "projection"),
		viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos"),
		//matAmbientLoc = glGetUniformLocation(lightingShader.Program, "material.ambient"),//material
		matDiffuseLoc = glGetUniformLocation(lightingShader.Program, "material.diffuse"),
		matSpecularLoc = glGetUniformLocation(lightingShader.Program, "material.specular"),
		matEmissionLoc = glGetUniformLocation(lightingShader.Program, "material.emission"),
		matShineLoc = glGetUniformLocation(lightingShader.Program, "material.shininess"),
		dirLightDirectionLoc = glGetUniformLocation(lightingShader.Program, "dirLight.direction"),
		pointLightConstantLoc = glGetUniformLocation(lightingShader.Program, "pointLight.constant"),
		pointLightLinearLoc = glGetUniformLocation(lightingShader.Program, "pointLight.linear"),
		pointLightQuadraticLoc = glGetUniformLocation(lightingShader.Program, "pointLight.quadratic"),
		lightAmbientLoc = glGetUniformLocation(lightingShader.Program, "light.ambient"),//light
		lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position"),
		lightDiffuseLoc = glGetUniformLocation(lightingShader.Program, "light.diffuse"),
		lightSpecularLoc = glGetUniformLocation(lightingShader.Program, "light.specular");
		
	lightShader.use();
	GLint	lightModelLoc = glGetUniformLocation(lightShader.Program, "model"),
		lightViewLoc = glGetUniformLocation(lightShader.Program, "view"),
		lightProjectionLoc = glGetUniformLocation(lightShader.Program, "projection");

	//game loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		do_movement();

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;




		//glClearColor(0.4f, 0.8f, 1.0f, 1.0f);//blue
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//texture
		
		
		
		//glUniform3fv(objectColorLoc, 1, glm::value_ptr(objectColor));
		//
		view = camera.getLookat();
		projection = glm::perspective(glm::radians(camera.info.zoom), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);
		

		/*
		glBindVertexArray(VAO);
		shader2.use();
		GLint	modelLoc = glGetUniformLocation(shader2.Program, "model"),
		viewLoc = glGetUniformLocation(shader2.Program, "view"),
		projectionLoc = glGetUniformLocation(shader2.Program, "projection"),
		texture1Loc = glGetUniformLocation(shader2.Program, "Texture1"),
		//texture2Loc = glGetUniformLocation(shader2.Program, "Texture2");
		glActiveTexture(GL_TEXTURE0);
		textureManager.BindTexture(0);
		glUniform1i(texture1Loc, 0);
		//glActiveTexture(GL_TEXTURE1);
		//textureManager.BindTexture(1);
		//glUniform1i(texture2Loc, 1);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 model;
		model = glm::translate(model, cubePositions[3]);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		*/

		//cube
		glBindVertexArray(VAO);	
		lightingShader.use();

		glUniformMatrix4fv(lightingViewLoc, 1, GL_FALSE, glm::value_ptr(view));//world
		glUniformMatrix4fv(lightingProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3f(viewPosLoc, camera.info.position.x, camera.info.position.y, camera.info.position.z);
		
		//glUniform3f(matAmbientLoc, 0.19225f, 0.19225f, 0.19225f);//material
		//glUniform3f(matDiffuseLoc, 0.50754f, 0.50754f, 0.50754f);
		glActiveTexture(GL_TEXTURE0);
		textureManager.BindTexture(0);
		glUniform1i(matDiffuseLoc, 0);
		glActiveTexture(GL_TEXTURE1);
		textureManager.BindTexture(1);
		glUniform1i(matSpecularLoc, 1);
		glActiveTexture(GL_TEXTURE2);
		textureManager.BindTexture(2);
		glUniform1i(matEmissionLoc, 2);

		glUniform1f(matShineLoc, 32.0f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);//light
		//glUniform3f(dirLightDirectionLoc, -0.2f, -1.0f, -0.3f);
		glUniform1f(pointLightConstantLoc, 1.0f);
		glUniform1f(pointLightLinearLoc, 0.09f);
		glUniform1f(pointLightQuadraticLoc, 0.032f);

		glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
		glUniform3f(lightDiffuseLoc, 0.8f, 0.8f, 0.8f);
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);



		glm::mat4 model;


		for (GLuint i = 0; i < 10; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(lightingModelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		/*
		model = glm::translate(model, cubePositions[0]);
		glUniformMatrix4fv(lightingModelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		*/
		glBindVertexArray(0);


		glBindVertexArray(lightVAO);
		//light
		lightShader.use();
		glUniformMatrix4fv(lightViewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(lightProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glUniformMatrix4fv(lightModelLoc, 1, GL_FALSE, glm::value_ptr(lightModel));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		
		


		outputDebugInfo();
		CalculateFrameRate();
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
			camera.setDefault();
			//camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
			break;
		case GLFW_KEY_F5:
			//camera.outputDebugInfo();
			//calc = true;
			//cout << deltaTime << "," << 1.0f / deltaTime << endl;
			//camera.rotateView(glm::radians(0.0f), glm::radians(0.0f), glm::radians(20.0f));
			//camera.rotateView(1.0f, 0.0f, 0.0f);
			//camera.outputDebugInfo();
			break;
		case GLFW_KEY_W:
		case GLFW_KEY_S:
		case GLFW_KEY_A:
		case GLFW_KEY_D:
		case GLFW_KEY_Q:
		case GLFW_KEY_E:
		case GLFW_KEY_LEFT_SHIFT:
		case GLFW_KEY_SPACE:
			keys[key] = true; break;
		default:return;
		}

	}
	if (action == GLFW_RELEASE) {
		keys[key] = false;
	}
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	GLdouble xoffset = lastX - xpos;
	GLdouble yoffset = lastY - ypos;
	camera.processMouseMovement(yoffset, xoffset);
	lastX = xpos;
	lastY = ypos;
}

void do_movement() {
	GLfloat cameraSpeed = 5.0f * deltaTime;
	if (keys[GLFW_KEY_W])
		camera.processKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.processKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.processKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.processKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_Q])
		camera.processKeyboard(LEFTROLLING, deltaTime);
	if (keys[GLFW_KEY_E])
		camera.processKeyboard(RIGHTROLLING, deltaTime);
	if (keys[GLFW_KEY_LEFT_SHIFT])
		camera.processKeyboard(DOWN, deltaTime);
	if (keys[GLFW_KEY_SPACE])
		camera.processKeyboard(UP, deltaTime);
	
}

void CalculateFrameRate()
{
	static float framesPerSecond = 0.0f;       // This will store our fps
	static float lastTime = 0.0f;       // This will hold the time from the last frame
	float currentTime = GetTickCount() * 0.001f;
	++framesPerSecond;
	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
		COORD temp = moveCursor(5, 6);

		cout << framesPerSecond;
		SetConsoleCursorPosition(hStdout, temp);
		framesPerSecond = 0;
		//calc = false;
	}
}


void drawAxisInit() {


}

void outputDebugInfo() {
	COORD temp = moveCursor(6,6);
	outputVec3(camera.info.position);
	moveCursor(7, 6);
	outputVec3(camera.info.view);
	moveCursor(8, 6);
	outputVec3(camera.info.up);
	moveCursor(9, 6);
	cout << camera.info.x1 << "          ";;
	moveCursor(10, 6);
	cout << camera.info.y1 << "          ";;
	moveCursor(11, 6);
	cout << camera.info.z1 << "          ";;
	moveCursor(12, 6);
	cout << camera.info.zoom << "          ";;
	SetConsoleCursorPosition(hStdout, temp);
}

COORD moveCursor(int row, int column) {
	COORD cursorPos;
	cursorPos.X = column;
	cursorPos.Y = row;
	CONSOLE_SCREEN_BUFFER_INFO lastPos;
	GetConsoleScreenBufferInfo(hStdout, &lastPos);
	SetConsoleCursorPosition(hStdout, cursorPos);
	cursorPos.X = lastPos.dwCursorPosition.X;
	cursorPos.Y = lastPos.dwCursorPosition.Y;
	return cursorPos;
}

inline void outputVec3(glm::vec3 vec) {
	cout << vec.x << "," << vec.y << "," << vec.z << "                                   ";
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScoll(yoffset);
}

bool useResource(int id,char *type,char *filename) {
	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(id), TEXT(type));
	if (NULL == hRsrc)
		return FALSE;
	DWORD dwSize = SizeofResource(NULL, hRsrc);
	if (0 == dwSize)
		return FALSE;
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	if (NULL == hGlobal)
		return FALSE;
	LPVOID pBuffer = LockResource(hGlobal);
	if (NULL == pBuffer)
		return FALSE;

	BOOL bRt = FALSE;
	FILE* fp = _tfopen(_T(filename), _T("wb"));
	if (fp != NULL)
	{
		if (dwSize == fwrite(pBuffer, sizeof(char), dwSize, fp))
			bRt = TRUE;
		fclose(fp);
	}

	//FreeResource(hGlobal);
	return bRt;
}