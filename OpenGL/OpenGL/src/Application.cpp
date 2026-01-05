//http://glew.sourceforge.net/
//사이트에서 GLEW 바이너리 버전다운로드. 필요 라이브러리 dependencies에 복사 후 설정
//http://glew.sourceforge.net/basic.html
//아래 예제 코드실행 확인 

//GLEW_STATIC Define 필요
#include <GL/glew.h> //glfw보다 먼저 include해야 함
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "Model.h"
#include "Light.h"
#include "Material.h"
#include "Manager/TimeManager.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

//glm::mat4 getProjectionMatrix(float fovy, float aspect, float n, float f) {
//	float half{ 1 / glm::tan(fovy, 2) };
//	glm::mat2 P{};
//
//
//}

void ChangeProgramAndMaterial(int&, int&, const bool*);

int main(void)
{
	Window mainWindow{ 800, 600 };
	mainWindow.Initialize();

	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		//std::array<float, 16> positions{
		//	-0.5f, -0.5f, 0.0f, 0.0f,
		//	0.5f, -0.5f, 1.0f, 0.0f,
		//	0.5f, 0.5f, 1.0f, 1.0f,
		//	-0.5f, 0.5f, 0.0f, 1.0f };

		//std::array<unsigned int, 6> indices{
		//	0, 1, 2,
		//	2, 3, 0
		//};

		//VertexArray va;
		//VertexBuffer vb{ positions.data(), 4 * 4 * sizeof(float) };

		//VertexBufferLayout layout;
		//layout.push<float>(2); // vertex당 2개의 위치를 포현하는 float 데이터
		//layout.push<float>(2); // vertex당 2개의 텍스쳐 좌표를 표현하는 float 데이터

		////layout.push<float>(3);
		//va.AddBuffer(vb, layout);

		//IndexBuffer ib{ indices.data(), 6 };

		Model teapot;
		teapot.LoadModel("res/models/teapot.obj");

		Camera camera{ glm::vec3{0.0f, 0.0f, 5.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, -90.0f, 0.0f, 5.0f, 0.5f };

		// Model view projection matrix 전달
		glm::mat4 model{ glm::translate(glm::mat4{1.0f}, glm::vec3{0.0f, 0.0f, 0.0f}) };
		float aspect{ static_cast<float>(mainWindow.GetBufferWidth()) / mainWindow.GetBufferHeight() };
		glm::mat4 proj{ glm::perspective(glm::radians(45.0f), aspect, 1.0f, 100.0f) };

		Shader shaderPerVertex{ "res/shaders/Lighting_Specular_Per_Vertex.shader" };
		shaderPerVertex.Bind();

		Shader shaderPerFragment{ "res/shaders/Lighting_Specular_Per_Fragment.shader" };
		shaderPerFragment.Bind();

		Texture texture{ "res/textures/uvchecker.jpg" };
		texture.Bind(); // 0번 슬롯에 바인딩
		shaderPerVertex.SetUniform1i("u_Texture", 0); // 0번 슬롯의 텍스처를 사용할 것이라는 것을 셰이더에 명시

		Renderer renderer;
		
		Light mainLight{ glm::vec3{1.0f, 1.0f, 1.0f}, 0.2f, glm::vec3{2.0f, -1.0f, -2.0f}, 1.0f };

		std::vector<Material> materials;
		materials.emplace_back(5.0f, 32.0f);
		materials.emplace_back(0.3f, 4.0f);

		int showObjectNum{ 0 };
		int materialNum{ 0 };

		TimeManager::GetInstance().Initialize();

		/* Loop until the user closes the window */
		while (!mainWindow.GetShouldClose())
		{
			TimeManager::GetInstance().Update();

			camera.KeyControl(mainWindow.GetKeys());
			camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

			ChangeProgramAndMaterial(showObjectNum, materialNum, mainWindow.GetKeys());

			/* Render here */
			renderer.Clear();

			if (showObjectNum == 0)
			{
				shaderPerVertex.Bind();
				mainLight.UseLight(shaderPerVertex);

				materials[materialNum].UseMaterial(shaderPerVertex);

				//shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
				shaderPerVertex.SetUniformMat4f("u_Model", model);
				shaderPerVertex.SetUniformMat4f("u_View", camera.CalculateViewMatrix());
				shaderPerVertex.SetUniformMat4f("u_Proj", proj);
				shaderPerVertex.SetUniform3f("u_EyePosition", camera.GetEyePosition().x, camera.GetEyePosition().y, camera.GetEyePosition().z);

				teapot.RenderModel(shaderPerVertex);
				shaderPerVertex.UnBind();
			}
			else if (showObjectNum == 1)
			{
				shaderPerFragment.Bind();
				mainLight.UseLight(shaderPerFragment);
				materials[materialNum].UseMaterial(shaderPerFragment);

				shaderPerFragment.SetUniformMat4f("u_Model", model);
				shaderPerFragment.SetUniformMat4f("u_View", camera.CalculateViewMatrix());
				shaderPerFragment.SetUniformMat4f("u_Proj", proj);
				shaderPerFragment.SetUniform3f("u_EyePosition", camera.GetEyePosition().x, camera.GetEyePosition().y, camera.GetEyePosition().z);

				teapot.RenderModel(shaderPerFragment);
				shaderPerFragment.UnBind();
			}

			/* Swap front and back buffers */
			mainWindow.SwapBuffers();

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	return 0;
}

void ChangeProgramAndMaterial(int& num, int& materialNum, const bool* keys)
{
	if (keys[GLFW_KEY_E])
	{
		num = 1;
	}

	if (keys[GLFW_KEY_Q])
	{
		num = 0;
	}

	if (keys[GLFW_KEY_Z])
	{
		materialNum = 0;
	}

	if (keys[GLFW_KEY_X])
	{
		materialNum = 1;
	}
}
