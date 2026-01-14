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
#include "ShadowMap.h"
#include "Manager/TimeManager.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

void ChangeScene(int&, const bool*);

int main(void)
{
	int bufferWidth{ 1920 };
	int bufferHeight{ 1080 };

	Window mainWindow{ bufferWidth, bufferHeight };
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
		shaderPerFragment.SetUniformMat4f("u_Projection", proj);
		shaderPerFragment.UnBind();

		// depth map을 텍스처로 활용하는 shader 작성
		Shader simpleDepthVisualizeShader{ "res/shaders/DepthMapVisualize.shader" };
		simpleDepthVisualizeShader.Bind();
		simpleDepthVisualizeShader.SetUniformMat4f("u_Projection", proj);
		simpleDepthVisualizeShader.UnBind();

		// depth map을 생성하는 shader
		Shader shaderShadowMap{ "res/shaders/DirectionalShadowMap.shader" };

		Texture texture{ "res/textures/uvchecker.jpg" };
		texture.Bind(); // 0번 슬롯에 바인딩

		Renderer renderer;
		
		DirectionalLight mainLight{ glm::vec3{1.0f, 1.0f, 1.0f}, 0.3f, 0.3f, 256, 256, glm::vec3{2.0f, -2.0f, 0.0f} };

		std::vector<Material> materials;
		materials.emplace_back(5.0f, 32.0f);
		materials.emplace_back(0.3f, 4.0f);

		int showObjectNum{ 0 };
		int materialNum{ 0 };
		int sceneNum{ 0 };

		TimeManager::GetInstance().Initialize();

		/* Loop until the user closes the window */
		while (!mainWindow.GetShouldClose())
		{
			TimeManager::GetInstance().Update();

			camera.KeyControl(mainWindow.GetKeys());
			camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

			ChangeScene(sceneNum, mainWindow.GetKeys());

			glm::vec3 camPosition{ camera.GetEyePosition() };

			// shadow map
			{
				int width{ mainLight.GetShadowMap().GetShadowWidth() };
				int height{ mainLight.GetShadowMap().GetShadowWidth() };

				mainWindow.ChangeViewPort(width, height);
				mainLight.GetShadowMap().Bind();
				renderer.Clear();

				shaderShadowMap.Bind();
				mainLight.UseLightForShadow(shaderShadowMap);

				shaderShadowMap.SetUniformMat4f("u_Model", glm::scale(glm::mat4{ 1.0f }, glm::vec3{ 0.3f, 0.3f, 0.3f }));
				teapot.RenderModel(shaderShadowMap);

				glm::mat4 planeModeMat{ glm::mat4{1.0f} };
				planeModeMat = glm::scale(planeModeMat, glm::vec3{ 0.5f, 0.5f, 0.5f });
				planeModeMat = glm::translate(planeModeMat, glm::vec3{ 0.0f, -10.0f, 0.0f });
				shaderShadowMap.SetUniformMat4f("u_Model", planeModeMat);
				plane.RenderModel(shaderShadowMap);

				mainLight.GetShadowMap().UnBind();
			}

			if(sceneNum == 0)
			{
				renderer.Clear();
				mainWindow.ChangeViewPort(bufferWidth, bufferHeight);
				shaderPerFragment.Bind();

				mainLight.GetShadowMap().Read(1);
				mainLight.UseLight(shaderPerFragment, 1);
				materials[0].UseMaterial(shaderPerFragment);

				shaderPerFragment.SetUniformMat4f("u_Model", glm::scale(glm::mat4{ 1.0f }, glm::vec3{ 0.3f, 0.3f, 0.3f }));
				shaderPerFragment.SetUniformMat4f("u_View", camera.CalculateViewMatrix());
				shaderPerFragment.SetUniform3f("u_EyePosition", camPosition.x, camPosition.y, camPosition.z);

				texture.Bind(0);
				shaderPerFragment.SetUniform1i("u_Texture", 0);
				teapot.RenderModel(shaderPerFragment);

				glm::mat4 planeModelMat{ glm::mat4{1.0f} };
				planeModelMat = glm::scale(planeModelMat, glm::vec3{ 0.5f, 0.5f, 0.5f });
				planeModelMat = glm::translate(planeModelMat, glm::vec3{ 0.0f, -10.0f, 0.0f });
				shaderPerFragment.SetUniformMat4f("u_Model", planeModelMat);
				materials[1].UseMaterial(shaderPerFragment);
				plane.RenderModel(shaderPerFragment);
				shaderPerFragment.UnBind();
			}
			else if (sceneNum == 1)
			{
				renderer.Clear();
				mainWindow.ChangeViewPort(bufferWidth, bufferHeight);
				simpleDepthVisualizeShader.Bind();
				mainLight.GetShadowMap().Read(1);

				simpleDepthVisualizeShader.SetUniformMat4f("u_Model", glm::rotate(glm::scale(glm::mat4{ 1.0f }, glm::vec3{ 0.1f, 0.1f, 0.1f }), 90.0f, glm::vec3{ 1.0f,0.0f,0.0f })); //Mat4{1.0}은 단위 행렬
				simpleDepthVisualizeShader.SetUniformMat4f("u_View", camera.CalculateViewMatrix());
				simpleDepthVisualizeShader.SetUniform1i("u_DepthMap", 1);

				plane.RenderModel(simpleDepthVisualizeShader);
			}

			/* Swap front and back buffers */
			mainWindow.SwapBuffers();

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	return 0;
}

void ChangeScene(int& sceneNum, const bool* keys)
{
	if (keys[GLFW_KEY_1])
	{
		sceneNum = 0;
	}

	if (keys[GLFW_KEY_2])
	{
		sceneNum = 1;
	}
}
