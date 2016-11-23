#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <glfw/glfw3.h>
#include <freeImage/FreeImage.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include "Shader.h"
#include "Texture.h"

const float screenWidth = 800, screenHeight = 600;

