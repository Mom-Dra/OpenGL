#pragma once

#include <GL/glew.h>
#include <string>

class Texture
{
private:
	GLuint rendererID;
	std::string filePath;
	unsigned char* localBuffer;
	int width, height, bPP; //(bits per channel)

public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
};