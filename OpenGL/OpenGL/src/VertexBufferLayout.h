#pragma once

#include <vector>
#include <GL/glew.h>

#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	explicit VertexBufferElement(unsigned int type, unsigned int count, unsigned char normalized) : type{ type }, count{ count }, normalized{ normalized }
	{

	}

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}

		ASSERT(0);
		return 0;
	}
};

template <typename T>
struct always_false : std::false_type {};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> elements; // 하나의 layout은 여러개의 element를 가지고 있음
	// (ex position, normal, color...)
	unsigned int stride; // vertex 하나당 데이터가 얼마나 떨어져 있는지

	// vertex 마다 data가 다를 순 없다
	// 어떤 vertex가 position과 color
	// texture Coordinate를 가지고 있다 치면
	// 다른 vertex도 똑같이 이런 정보를 가지고 있어야 합니다
	// vertex 하나마다 가지고 있는 데이터의 크기는 동일하기 때문에
	// stride는 항상 같은거에요

public:
	explicit VertexBufferLayout() : stride{ 0 }
	{

	}

	template <typename T>
	void push(unsigned int count)
	{
		static_assert(always_false<T>::value, "This type is not supported");
	}

	template<>
	void push<float>(unsigned int count)
	{
		elements.emplace_back(GL_FLOAT, count, GL_FALSE);
		stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		elements.emplace_back(GL_UNSIGNED_INT, count, GL_FALSE);
		stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template <>
	void push<unsigned char>(unsigned int count)
	{
		elements.emplace_back(GL_UNSIGNED_BYTE, count, GL_TRUE);
		stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement>& GetElement() const { return elements; }
	inline unsigned int GetStride() const { return stride; }
};