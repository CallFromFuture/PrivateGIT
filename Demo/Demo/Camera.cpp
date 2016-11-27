#include "Camera.h"

using namespace glm;

Camera *Camera::m_inst(0);



Camera::Camera()
{
	updateVectors();
}


Camera * Camera::Inst()
{
	if (!m_inst)
		m_inst = new Camera();
	return m_inst;
}

Camera::~Camera()
{
	m_inst = 0;
}

mat4 Camera::getLookat()
{
	return lookAt(position, position + view, up);
}

void Camera::outputDebugInfo()
{
	std::cout << "pos:" << position.x << "," << position.y << "," << position.z << std::endl;
	std::cout << "view:" << view.x << "," << view.y << "," << view.z << std::endl;
	std::cout << "up:" << up.x << "," << up.y << "," << up.z << std::endl;

	std::cout << "x1:" << x1 << ",y1:" << y1 << ",z1:" << z1 << std::endl;
	//std::cout << radians(-89.0f) << std::endl;

}

/*
void Camera::setCameraData()
{
}
*/

void Camera::setDefault()
{
	up = vec3(0.0f, 1.0f, 0.0f);
	view = vec3(0.0f, 0.0f, -1.0f);
	position = vec3(0.0f, 0.0f, 3.0f);
	x1 = 0.0f;
	y1 = 0.0f;
	z1 = 0.0f;
	zoom = 45.0f;
	updateVectors();
}

void Camera::processMouseMovement(GLdouble xoffset, GLdouble yoffset)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	x1 += xoffset;
	y1 += yoffset;
	
		if (x1 > 89.5f|| x1 < -89.5f){
			x1 -= xoffset;
			rotateViewB(0, radians(yoffset), 0);
		}
		else {
			rotateViewB(radians(xoffset), radians(yoffset), 0);
		}
		if (y1 < -180.0f)
			y1 += 360.0f;
		else if (y1 > 180.0f)
			y1 -= 360.0f;
	updateVectors();
}

void Camera::processKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	GLfloat velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		position += view * velocity;
	if (direction == BACKWARD)
		position -= view * velocity;
	if (direction == LEFT)
		position -= right * velocity;
	if (direction == RIGHT)
		position += right * velocity;
	if (direction == DOWN)
		position -= up * velocity;
	if (direction == UP)
		position += up * velocity;

	if (direction == RIGHTROLLING)
		rotateViewB(0, 0, radians(-RollingSpeed));
	if (direction == LEFTROLLING)
		rotateViewB(0, 0, radians(RollingSpeed));
	updateVectors();
}

void Camera::processMouseScoll(GLfloat yoffset) {
	if (zoom >= 1.0f && zoom <= 45.0f)
		zoom -= yoffset;
	if (zoom <= 1.0f)
		zoom = 1.0f;
	if (zoom >= 45.0f)
		zoom = 45.0f;
	updateVectors();
}
void Camera::updateVectors()
{
	//float x = sqrt(1 - pow(view.z, 2));
	//up = vec3(x, -view.z, 0);

	right = glm::normalize(glm::cross(view, up));
	//2:right = glm::normalize(glm::cross(view, worldUp));

	//vec3 temp = glm::normalize(glm::cross(right, view));
	//std::cout << "UP1"<<temp.x<<","<<temp.y<<","<<temp.z << std::endl;
	//2:up = glm::normalize(glm::cross(right, view));

	info.position = position;
	info.view = view;
	info.up = up;
	info.x1 = x1;
	info.y1 = y1;
	info.z1 = z1;
	info.zoom = zoom;
	//std::cout << "UP2" << up.x << "," << up.y << "," << up.z << std::endl;
}
/*

void Camera::rotateView(GLfloat eularX, GLfloat eularY, GLfloat eularZ) {

	quat tempA(vec3(eularX, eularY, 0));
	quat tempView(0, view.x, view.y, view.z), result(1, 0, 0, 0);
	quat tempB = conjugate(normalize(tempA));
	result = tempA * tempView * tempB;
	view.x = result.x;
	view.y = result.y;
	view.z = result.z;
	quat tempC(vec3(0, 0, eularZ)), tempUp(0, up.x, up.y, up.z), result2(1, 0, 0, 0);
	result2 = tempC * tempUp;
	up.x = result2.x;
	up.y = result2.y;
	up.z = result2.z;


	updateVectors();
}

*/

void Camera::rotateViewB(GLfloat xoffset, GLfloat yoffset,GLfloat zoffset) {
	vec3 AxisX = cross(view , up);
	vec3 AxisY = up;
	//vec3 AxisZ = view;
	vec3 AxisZ = cross(AxisX, AxisY);
	AxisX = normalize(AxisX);
	AxisY = normalize(AxisY);
	AxisZ = normalize(AxisZ);
	rotateTest(xoffset, AxisX.x, AxisX.y, AxisX.z);
	rotateTest(yoffset, AxisY.x, AxisY.y, AxisY.z);
	rotateTestB(zoffset, AxisZ.x, AxisZ.y, AxisZ.z);
	z1 += degrees(zoffset);
	if (z1 < -180.0f)
		z1 += 360.0f;
	else if (z1 > 180.0f)
		z1 -= 360.0f;
	updateVectors();
}

double Camera::length(glm::quat quat)
{
	return sqrt(quat.x * quat.x + quat.y * quat.y +
		quat.z * quat.z + quat.w * quat.w);
}

glm::quat Camera::normalize(glm::quat quat) {
	double L = length(quat);
	quat.x /= L;
	quat.y /= L;
	quat.z /= L;
	quat.w /= L;
	return quat;
}

vec3 Camera::normalize(vec3 vec) {
	double length1 = sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
	vec3 ret;
	ret.x = vec.x / length1;
	ret.y = vec.y / length1;
	ret.z = vec.z / length1;
	return ret;
}

glm::quat Camera::conjugate(glm::quat quat) {
	quat.x = -quat.x;
	quat.y = -quat.y;
	quat.z = -quat.z;
	return quat;
}

void Camera::rotateTest(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
	quat temp, quat_view, result;

	temp.x = x * sin(angle / 2);
	temp.y = y * sin(angle / 2);
	temp.z = z * sin(angle / 2);
	temp.w = cos(angle / 2);

	quat_view.x = view.x;
	quat_view.y = view.y;
	quat_view.z = view.z;
	quat_view.w = 0;

	result = Mult(Mult(temp, quat_view), conjugate(temp));

	view.x = result.x;
	view.y = result.y;
	view.z = result.z;
}

void Camera::rotateTestB(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {

	quat temp, quat_up, result;

	temp.x = x * sin(angle / 2);
	temp.y = y * sin(angle / 2);
	temp.z = z * sin(angle / 2);
	temp.w = cos(angle / 2);

	quat_up.x = up.x;
	quat_up.y = up.y;
	quat_up.z = up.z;
	quat_up.w = 0;

	result = Mult(Mult(temp, quat_up), conjugate(temp));

	up.x = result.x;
	up.y = result.y;
	up.z = result.z;
}
quat Camera::Mult(quat A, quat B)
{
	quat C;

	C.x = A.w*B.x + A.x*B.w + A.y*B.z - A.z*B.y;
	C.y = A.w*B.y - A.x*B.z + A.y*B.w + A.z*B.x;
	C.z = A.w*B.z + A.x*B.y - A.y*B.x + A.z*B.w;
	C.w = A.w*B.w - A.x*B.x - A.y*B.y - A.z*B.z;
	return C;
}