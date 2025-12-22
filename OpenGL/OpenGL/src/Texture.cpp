#include "Texture.h"
#include "Renderer.h"
#include "vendor/stb_image/stb_image.h"
#include <iostream>

Texture::Texture(std::string_view path) : rendererID{ 0 }, filePath{ path }, width{ 0 }, height{ 0 }, bPP{ 0 }
{
	// 텍스처 위아래 뒤집음(OpenGL은 좌표계 다름)
	stbi_set_flip_vertically_on_load(1);
	localBuffer = stbi_load(path.data(), &width, &height, &bPP, 4); // 디스크에서 이미지 읽어오기

	if (!localBuffer)
	{
		std::cout << "no file : " << path << std::endl;
		return;
	}

	isLoaded = true;

	GLCall(glGenTextures(1, &rendererID)); // 텍스처 버퍼 생성
	GLCall(glBindTexture(GL_TEXTURE_2D, rendererID)); // 바인딩

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// 텍스처 데이터 전달
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0)); // 전달이 끝났으니 언바인딩

	if (localBuffer)
	{
		stbi_image_free(localBuffer); // 버퍼 free
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
