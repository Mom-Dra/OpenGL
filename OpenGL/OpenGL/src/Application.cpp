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
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

//glm::mat4 getProjectionMatrix(float fovy, float aspect, float n, float f) {
//	float half{ 1 / glm::tan(fovy, 2) };
//	glm::mat2 P{};
//
//
//}

int main(void)
{
	Window mainWindow{ 800, 600 };
	mainWindow.Initialize();

	{
		glEnable(GL_CULL_FACE);
		std::array<float, 12> positions{
			-0.5f, -0.5f, -5.0f,
			0.5f, -0.5f, -5.0f,
			0.5f, 0.5f, -5.0f,
			-0.5f, 0.5f, -5.0f, };

		std::array<unsigned int, 6> indices{
			0, 1, 2,
			2, 3, 0
		};

		VertexArray va;
		VertexBuffer vb{ positions.data(), 4 * 3 * sizeof(float) };

		VertexBufferLayout layout;
		layout.push<float>(3);
		//layout.push<float>(3);
		va.AddBuffer(vb, layout);

		IndexBuffer ib{ indices.data(), 6 };

		// Model view projection matrix 전달
		glm::mat4 model{ glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) };
		glm::mat4 proj{ glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 1.0f, 100.0f) };

		Shader shader{ "res/shaders/Basic.shader" };
		shader.Bind();

		Renderer renderer;

		va.UnBind();
		vb.UnBind();
		ib.UnBind();
		shader.UnBind();

		/* Loop until the user closes the window */
		while (!mainWindow.GetShouldClose())
		{
			/* Render here */
			renderer.Clear();

			//va.Bind();
			shader.Bind();
			shader.SetUniformMat4f("u_Model", model);
			shader.SetUniformMat4f("u_Proj", proj);
			renderer.Draw(va, ib, shader);

			//GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			/* Swap front and back buffers */
			mainWindow.SwapBuffers();

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}