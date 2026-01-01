#pragma once

#include <GL/glew.h>
#include <string>

class Texture
{
private:
	GLuint rendererID; // 이전과 마찬가지로 텍스처 객체에 대한 ID 저장
	std::string filePath; // 텍스처(이미지) 파일 경로
	unsigned char* localBuffer; // 텍스처 데이터
	int width, height, bPP; //(bits per channel)

	bool isLoaded{ false };

public:
	explicit Texture(std::string_view path);
	~Texture();

	void Bind(unsigned int slot = 0) const; // 텍스처를 바인딩할 슬롯, GPU에 따라 숫자 제한이 있음
	void Unbind() const;

	int GetWidth() const noexcept { return width; }
	int GetHeight() const noexcept { return height; }

	bool IsLoaded() const noexcept { return isLoaded; }
};