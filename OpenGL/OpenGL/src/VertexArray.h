#pragma once

#include <GL/glew.h>

class VertexBuffer;
class VertexBufferLayout;

class VertexArray
{
private:
	GLuint rendererID;

public:
	explicit VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void UnBind() const;
};
