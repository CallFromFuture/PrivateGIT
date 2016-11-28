#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:4244)
#pragma warning(disable:4049)
#include <windows.h>
#include <tchar.h>
#include "resource.h"
#define GLEW_STATIC


#include <GL/glew.h>
#include <glfw/glfw3.h>

//#include <freeImage/FreeImage.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <fstream>
#include <string>

using namespace std;
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
static volatile const int screenWidth = 800, screenHeight = 600;
static GLdouble lastX = screenWidth / 2.0, lastY = screenWidth / 2.0;


