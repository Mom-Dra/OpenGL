#pragma once

class ShadowMap
{
protected:
	unsigned int FBO;
	unsigned int shadowMap;
	int shadowWidth;
	int shadowHeight;

public:
	explicit ShadowMap(int width, int height);
	virtual ~ShadowMap();

	virtual void Bind();
	virtual void UnBind();
	virtual void Read(unsigned int slot);

	int GetShadowWidth() const noexcept { return shadowWidth; }
	int GetShadowHeight() const noexcept { return shadowHeight; }
};
