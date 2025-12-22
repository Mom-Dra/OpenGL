#pragma once

#include <string_view>
#include <memory>
#include <vector>

//#include "Shader.h"
//#include "Texture.h"
//#include "Renderer.h"

class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Texture;
class Shader;
struct aiNode;
struct aiScene;
struct aiMesh;

class Model
{
private:
	std::vector<std::unique_ptr<VertexArray>> VAOs;
	std::vector<std::unique_ptr<VertexBuffer>> VBOs;
	std::vector<std::unique_ptr<IndexBuffer>> IBOs;
	std::vector<std::unique_ptr<Texture>> textureList;
	std::vector<unsigned int> meshToTex;

public:
	explicit Model() = default;
	~Model() = default;

	void LoadModel(std::string_view fileName);
	void ClearModel();
	void RenderModel(const Shader& shader);

private:
	void LoadNode(aiNode* node, const aiScene& scene);
	void LoadMesh(aiMesh* mesh, const aiScene& scene);
	void LoadMaterials(const aiScene& scene);
};
