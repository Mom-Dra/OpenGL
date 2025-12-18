#pragma once

class IndexBuffer
{
private:
	unsigned int rendererID;
	unsigned int count;

public:
	explicit IndexBuffer(const unsigned int* data, unsigned int count);
	// index는 (대부분) unsigned int* 타입, count는 갯수 (size와 다름)
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;

	inline unsigned int GetCount() const { return count; }
};
