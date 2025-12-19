#include "Texture.h"
#include "Renderer.h"
#include "vendor/stb_image/stb_image.h"
#include <iostream>

Texture::Texture(const std::string& path) : rendererID{ 0 }, filePath{ path }, width{ 0 }, height{ 0 }, bPP{ 0 }
{
	stbi_set_flip_vertically_on_load(1);
	localBuffer = stbi_load(path.c_str(), &width, &height, &bPP, 4);
	if (!localBuffer)
	{
		std::cout << "������ �����ϴ� : " << path << std::endl;
		return;
	}

	GLCall(glGenTextures(1, &rendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, rendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (localBuffer)
	{
		stbi_image_free(localBuffer);
	}
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &rendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot)); //�ؽ�ó�� Ȱ��ȭ �� �� �ִ� ������ �����Ǿ� ������, �̸� �����ؾ� ��
	GLCall(glBindTexture(GL_TEXTURE_2D, rendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
