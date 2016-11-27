#pragma once
#include <glfw/glfw3.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

struct cameraInfo {
	glm::vec3 position, view, up;
	GLfloat x1, y1, z1, zoom;
};

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	LEFTROLLING,
	RIGHTROLLING,
	UP,
	DOWN
};
class Camera
{
public:
	GLfloat MovementSpeed = 3.0f;
	GLfloat MouseSensitivity = 0.05f;
	GLfloat RollingSpeed = 3.0f;
	

	//glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	static Camera *Inst();
	~Camera();
	glm::mat4 Camera::getLookat();
	void outputDebugInfo();
	//void setCameraData(glm::vec3 position,glm::vec3 up,glm::vec3 view,GLfloat speed,GLfloat sensitivty);
	void setDefault();
	void processMouseMovement(GLdouble xoffset, GLdouble yoffset);
	void processKeyboard(Camera_Movement direction, GLfloat deltaTime);
	void processMouseScoll(GLfloat yoffset);
	//void processKeyboard(Camera_Movement direction, GLfloat deltaTime);
	void updateVectors();
	//void rotateView(GLfloat eularX, GLfloat eularY, GLfloat eularZ);
	//void rotateViewA(GLfloat eularX, GLfloat eularY, GLfloat eularZ);
	void rotateViewB(GLfloat xoffset, GLfloat yoffset, GLfloat zoffset);
	cameraInfo info;
	
private:
	
	
	Camera();
	static Camera *m_inst;
	GLfloat x1 = 0.0f, y1 = 0.0f, z1 = 0.0f;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), view = glm::vec3(0.0f, 0.0f, -1.0f), right;//, up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	GLfloat zoom = 45.0f;

	double length(glm::quat quat);
	glm::quat normalize(glm::quat quat);
	glm::vec3 normalize(glm::vec3 vec);
	glm::quat conjugate(glm::quat quat);
	void rotateTest(GLfloat angle, GLfloat X, GLfloat Y, GLfloat Z);
	void rotateTestB(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
	glm::quat Mult(glm::quat A, glm::quat B);

};

