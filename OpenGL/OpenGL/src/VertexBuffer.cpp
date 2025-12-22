#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLCall(glGenBuffers(1, &rendererID)); // 1. 버퍼 생성
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID)); // 2. 바인딩("작업 상태")
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); // 3. 작업 상태 버퍼에 데이터 전달
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &rendererID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
}

void VertexBuffer::UnBind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
