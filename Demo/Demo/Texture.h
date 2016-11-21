#pragma once
#include <GL/glew.h>
#include <freeImage/FreeImage.h>
#include <map>
#include <iostream>
class Texture
{
public:
	static Texture *Inst();
	~Texture();

	bool loadTexture(const char* filename, const unsigned int texID, GLenum image_format = GL_RGB, GLint internal_format = GL_RGB, GLint level = 0, GLint border = 0);
	
	bool unloadTexture(const char texID);
	bool BindTexture(const unsigned int texID);

	void unloadAlltextures();

private:
	Texture();
	static Texture *m_inst;
	std::map<unsigned int, GLuint> m_texID;
};

