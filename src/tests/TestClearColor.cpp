#include "TestClearColor.h"

#include "Renderer.h"
#include "imgui/imgui.h"
// Triangle points
float positions[] = {
	//   x,       y,      tc x, tc y
		 -50.0f, -50.0f, 0.0f, 0.0f, // 0
		  50.0f, -50.0f, 1.0f, 0.0f, // 1
		  50.0f,  50.0f, 1.0f, 1.0f, // 2
		 -50.0f,  50.0f, 0.0f, 1.0f  // 3
};

unsigned int indices[] = {
	0, 1, 2,
	2, 3, 0
};



namespace test {
	TestClearColor::TestClearColor()
		: m_ClearColor{0.2f, 0.3f, 0.8f, 1.0f}
	{
	}

	TestClearColor::~TestClearColor()
	{
	}

	void TestClearColor::OnUpdate(float dt)
	{
	}

	void TestClearColor::OnRender()
	{
		GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
	}




}