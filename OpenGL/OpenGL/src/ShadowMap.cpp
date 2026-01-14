#include "ShadowMap.h"
#include "Renderer.h"
#include <iostream>
#include <array>

ShadowMap::ShadowMap(int width, int height) : shadowWidth{ width }, shadowHeight{ height }
{
	GLCall(glGenFramebuffers(1, &FBO));

	GLCall(glGenTextures(1, &shadowMap));
	GLCall(glBindTexture(GL_TEXTURE_2D, shadowMap));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	std::array<float, 4> borderColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor.data()));

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, FBO));

	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0));

	GLCall(glDrawBuffer(GL_NONE));
	GLCall(glReadBuffer(GL_NONE));

	GLCall(int status = glCheckFramebufferStatus(GL_FRAMEBUFFER));

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "프레임 버퍼 오류: " << status << std::endl;
		return;
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

ShadowMap::~ShadowMap()
{
	if (FBO != 0)
		GLCall(glDeleteFramebuffers(1, &FBO));

	if (shadowMap != 0)
		GLCall(glDeleteTextures(1, &FBO));
}

void ShadowMap::Bind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, FBO));
}

void ShadowMap::UnBind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void ShadowMap::Read(unsigned int slot)
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, shadowMap));
}
