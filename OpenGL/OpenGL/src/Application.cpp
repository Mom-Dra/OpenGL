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
#include "DirectionalLight.h"
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
		Model teapot;
		teapot.LoadModel("res/models/teapot.obj");

		Model plane;
		plane.LoadModel("res/models/SubdividedPlane_100.obj");

		Camera camera{ glm::vec3{0.0f, 0.0f, 5.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, -90.0f, 0.0f, 5.0f, 0.5f };

		glm::mat4 model{ glm::translate(glm::mat4{1.0f}, glm::vec3{0.0f, 0.0f, 0.0f}) };
		float aspect{ static_cast<float>(mainWindow.GetBufferWidth()) / mainWindow.GetBufferHeight() };
		glm::mat4 proj{ glm::perspective(glm::radians(45.0f), aspect, 1.0f, 100.0f) };

		Shader shaderPerFragment{ "res/shaders/Lighting_Specular_Per_Fragment.shader" };
		shaderPerFragment.Bind();

		Shader shaderNormalMap{ "res/shaders/Lighting_And_NormalMap.shader" };
		shaderNormalMap.Bind();

		Texture texture{ "res/textures/diffuse.png" };
		//texture.Bind(0); // 0번 슬롯에 바인딩
		//shaderPerFragment.SetUniform1i("u_Texture", 0); // 0번 슬롯의 텍스처를 사용할 것이라는 것을 셰이더에 명시

		Texture normalMap{ "res/textures/normal.png" };

		/*Shader simpleTransparent{ "res/shaders/SimpleTransparent.shader" };
		simpleTransparent.Bind();

		glm::mat4 planeModeMat{ glm::mat4{ 1.0f } };
		planeModeMat = glm::scale(planeModeMat, glm::vec3{ 0.1f, 0.1f, 0.1f });
		planeModeMat = glm::rotate(planeModeMat, glm::radians(90.0f), glm::vec3{ 1.0f, 0.0f, 0.0f });
		planeModeMat = glm::translate(planeModeMat, glm::vec3{ 0.0f, 110.0f, 0.0f });
		simpleTransparent.SetUniformMat4f("u_Model", planeModeMat);
		simpleTransparent.SetUniformMat4f("u_Proj", proj);
		simpleTransparent.SetUniformMat4f("u_View", camera.CalculateViewMatrix());
		simpleTransparent.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 0.5f);*/

		Renderer renderer;

		DirectionalLight mainLight{ glm::vec3{1.0f, 1.0f, 1.0f}, 0.3f, glm::vec3{2.0f, -1.0f, -2.0f}, 0.3f };

		std::vector<Material> materials;
		materials.emplace_back(5.0f, 32.0f);
		materials.emplace_back(0.3f, 4.0f);

		int showObjectNum{ 0 };
		int materialNum{ 0 };
		int shaderNum{ 0 };

		TimeManager::GetInstance().Initialize();

		/* Loop until the user closes the window */
		while (!mainWindow.GetShouldClose())
		{
			TimeManager::GetInstance().Update();

			camera.KeyControl(mainWindow.GetKeys());
			camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

			ChangeProgramAndMaterial(materialNum, shaderNum, mainWindow.GetKeys());

			/* Render here */
			renderer.Clear();

			if (shaderNum == 0)
			{
				shaderPerFragment.Bind();
				mainLight.UseLight(shaderPerFragment);
				materials[materialNum].UseMaterial(shaderPerFragment);
				glm::mat4 planeModeMat{ glm::mat4{1.0} };

				planeModeMat = glm::rotate(planeModeMat, 90.0f, glm::vec3{ 1.0f, 0.0f, 0.0f });
				planeModeMat = glm::scale(planeModeMat, glm::vec3{ 0.1f, 0.1f, 0.1f });

				shaderPerFragment.SetUniformMat4f("u_Model", planeModeMat);
				shaderPerFragment.SetUniformMat4f("u_View", camera.CalculateViewMatrix());
				shaderPerFragment.SetUniformMat4f("u_Proj", proj);
				shaderPerFragment.SetUniform3f("u_EyePosition", camera.GetEyePosition().x, camera.GetEyePosition().y, camera.GetEyePosition().z);
				texture.Bind();
				shaderPerFragment.SetUniform1i("u_Texture", 0);
				/*normalMap.Bind(1);
				shaderPerFragment.SetUniform1i("u_Normal", 1);*/
				plane.RenderModel(shaderPerFragment);
				shaderPerFragment.UnBind();
			}
			else if (shaderNum == 1)
			{
				shaderNormalMap.Bind();
				mainLight.UseLight(shaderNormalMap);
				materials[materialNum].UseMaterial(shaderNormalMap);
				glm::mat4 planeModeMat{ glm::mat4{1.0} };

				planeModeMat = glm::rotate(planeModeMat, 90.0f, glm::vec3{ 1.0f, 0.0f, 0.0f });
				planeModeMat = glm::scale(planeModeMat, glm::vec3{ 0.1f, 0.1f, 0.1f });

				shaderNormalMap.SetUniformMat4f("u_Model", planeModeMat);
				shaderNormalMap.SetUniformMat4f("u_View", camera.CalculateViewMatrix());
				shaderNormalMap.SetUniformMat4f("u_Proj", proj);
				shaderNormalMap.SetUniform3f("u_EyePosition", camera.GetEyePosition().x, camera.GetEyePosition().y, camera.GetEyePosition().z);
				texture.Bind();
				shaderNormalMap.SetUniform1i("u_Texture", 0);
				normalMap.Bind(1);
				shaderNormalMap.SetUniform1i("u_Normal", 1);
				plane.RenderModel(shaderNormalMap);
				shaderNormalMap.UnBind();
			}

			/* Swap front and back buffers */
			mainWindow.SwapBuffers();

			/* Poll for and process events */
			glfwPollEvents();	
		}
	}

	return 0;
}

void ChangeProgramAndMaterial(int& materialNum, int& shaderNum, const bool* keys)
{
	if (keys[GLFW_KEY_Z])
	{
		materialNum = 0;
	}

	if (keys[GLFW_KEY_X])
	{
		materialNum = 1;
	}

	if (keys[GLFW_KEY_1])
	{
		shaderNum = 0;
	}

	if (keys[GLFW_KEY_2])
	{
		shaderNum = 1;
	}
}
