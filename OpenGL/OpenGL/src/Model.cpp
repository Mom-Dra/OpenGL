#include "Model.h"
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Texture.h"

void Model::LoadModel(std::string_view fileName)
{
	Assimp::Importer importer;
	const aiScene* scene{ importer.ReadFile(fileName.data(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals) };

	if (!scene)
	{
		std::cout << "Model Load Fail: " << fileName << ' ' << importer.GetErrorString() << std::endl;
		return;
	}

	LoadNode(scene->mRootNode, *scene);
	LoadMaterials(*scene);
}

void Model::ClearModel()
{

}

void Model::LoadNode(aiNode* node, const aiScene& scene)
{
	for (unsigned int i{ 0 }; i < node->mNumMeshes; ++i)
	{
		LoadMesh(scene.mMeshes[node->mMeshes[i]], scene);
	}

	for (unsigned int i{ 0 }; i < node->mNumChildren; ++i)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh* mesh, const aiScene& scene)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	// parse vertex info
	for (size_t i{ 0 }; i < mesh->mNumVertices; ++i)
	{
		// vertex position
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });

		// vertex uv
		if (mesh->mTextureCoords[0])
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		else
			vertices.insert(vertices.end(), { 0.0f, 0.0f });

		// vertex normal
		vertices.insert(vertices.end(), { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z });
	}

	// index info parse
	for (size_t i{ 0 }; i < mesh->mNumFaces; ++i)
	{
		aiFace face{ mesh->mFaces[i] };

		for (size_t j{ 0 }; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	std::unique_ptr<VertexArray> VAO{ std::make_unique<VertexArray>() };
	std::unique_ptr<VertexBuffer> vb{ std::make_unique<VertexBuffer>(&vertices[0], mesh->mNumVertices * 8 * sizeof(float)) };

	VertexBufferLayout layout;
	layout.push<float>(3); // vertex당 3개의 위치를 표현하는 float 데이터
	layout.push<float>(2); // vertex당 2개의 텍스쳐 좌표를 표현하는 float 데이터
	layout.push<float>(3); // vertex당 3개의 법선 벡터를 표현하는 float 데이터

	VAO->AddBuffer(*vb, layout);
	VAOs.emplace_back(std::move(VAO));
	VBOs.emplace_back(std::move(vb));

	IBOs.emplace_back(std::make_unique<IndexBuffer>(&indices[0], mesh->mNumFaces * 3));
}

void Model::LoadMaterials(const aiScene& scene)
{
	textureList.reserve(scene.mNumMaterials);

	std::cout << "numMaterials: " << scene.mNumMaterials << '\n';

	for (size_t i{ 0 }; i < scene.mNumMaterials; ++i)
	{
		const aiMaterial& material{ *scene.mMaterials[i] };

		bool isTextureAdded{ false };
		
		if (material.GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			
			if (material.GetTexture(aiTextureType_DIFFUSE, 0, &path) == aiReturn_SUCCESS)
			{
				size_t idx{ std::string(path.data).rfind("\\") };
				std::string fileName{ std::string(path.data).substr(idx + 1) };
				std::cout << "fileName: " << fileName << '\n';
				std::string texPath{ std::string("textures/").append(fileName) };

				std::unique_ptr<Texture> texture{ std::make_unique<Texture>(texPath) };

				if(texture->IsLoaded())
				{
					isTextureAdded = true;
					std::cout << "texture load success: " << texPath << std::endl;
					textureList.emplace_back(std::move(texture));
				}
			}
		}

		if(!isTextureAdded)
		{
			std::cout << "textre load failed\n";
			textureList.emplace_back(std::make_unique<Texture>("res/textures/steve.jpg"));
		}
	}
}

void Model::RenderModel(const Shader& shader)
{
	for (size_t i{ 0 }; i < VAOs.size(); ++i)
	{
		Renderer r;
		r.Draw(*VAOs[i], *IBOs[i], shader);
	}
}
