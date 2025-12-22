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

	bool isLoaded{ false };

public:
	explicit Texture(std::string_view path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	int GetWidth() const noexcept { return width; }
	int GetHeight() const noexcept { return height; }

	bool IsLoaded() const noexcept { return isLoaded; }
};