#include "Texture.h"

Texture *Texture::m_inst(0);

Texture::Texture() {
	FreeImage_Initialise();
}

Texture *Texture::Inst() {
	if (!m_inst)
		m_inst = new Texture();
	return m_inst;
}

Texture::~Texture() {
	FreeImage_DeInitialise();
	unloadAlltextures();
	m_inst = 0;
}

bool Texture::loadTexture(const char *filename, const unsigned int texID, GLenum image_format, GLint internal_format, GLint level, GLint border) {
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP *dib(0);
	BYTE *bits(0);

	unsigned int width(0), height(0);
	GLuint gl_texID;
	//read file
	fif = FreeImage_GetFileType(filename, 0);
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	if (fif == FIF_UNKNOWN)
		return false;
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	if (!dib)
		return false;
	//get data
	bits = FreeImage_GetBits(dib);
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//must not be empty
	if ((bits == 0) || (width == 0) || (height == 0))
		return false;
	//if exist, delete it
	if (m_texID.find(texID) != m_texID.end())
		glDeleteTextures(1, &(m_texID[texID]));
	//generate texture
	glGenTextures(1, &gl_texID);
	m_texID[texID] = gl_texID;
	glBindTexture(GL_TEXTURE_2D, gl_texID);
	glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height, border, image_format, GL_UNSIGNED_BYTE, bits);
	glGenerateMipmap(GL_TEXTURE_2D);
	//unload image
	FreeImage_Unload(dib);
	//return true
	return true;
}

bool Texture::unloadTexture(const char texID)
{
	//if exist, delete it
	if (m_texID.find(texID) != m_texID.end()) {
		glDeleteTextures(1, &(m_texID[texID]));
		m_texID.erase(texID);
		return true;
	}
	//otherwise, unload failed
	else return false;
}

bool Texture::BindTexture(const unsigned int texID)
{
	//if exist, bind it
	if (m_texID.find(texID) != m_texID.end())
		glBindTexture(GL_TEXTURE_2D, m_texID[texID]);
	//otherwise, binding failed
	else return false;
}

void Texture::unloadAlltextures()
{
	std::map<unsigned int, GLuint>::iterator end = m_texID.end();
	//unload the textures
	for(auto iter = m_texID.begin(); iter != end; ++iter) {
		glDeleteTextures(1, &(iter->second));	
	}
	m_texID.clear();
}
