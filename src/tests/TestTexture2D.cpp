#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace test {
	TestTexture2D::TestTexture2D()
        : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), 
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0))),
        m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
	{
        float positions[] = {
             100.0f,  100.0f, 0.0f, 0.0f, // 0
             200.0f,  100.0f, 1.0f, 0.0f, // 1
             200.0f,  200.0f, 1.0f, 1.0f, // 2
             100.0f,  200.0f, 0.0f, 1.0f  // 3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // Setup blending
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        m_VAO = std::make_unique<VertexArray>();

        //VertexArray vao; // Create Vertex Array Buffer
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float)); // Create vertex buffer

        VertexBufferLayout attribLayout; // create the attribute pointer buffer
        attribLayout.Push<float>(2);  // add pointers to attribute buffer
        attribLayout.Push<float>(2);  // add texture coordinates

        m_VAO->AddBuffer(*m_VertexBuffer, attribLayout); // add the vertex buffer and attribute layout to he vertex attribute array
        m_IBO = std::make_unique<IndexBuffer>(indices, 6);
       

        // Projection Matrix - pixel space
        //m_Proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        //// View Matrix - moving everything left 100 pixels
        //m_View = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));


        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->Bind();

        // Setup texture
        m_Texture = std::make_unique<Texture>("res/textures/xmas_text_1.png");
        m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float dt)
	{
	}

	void TestTexture2D::OnRender(float zoom)
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_View = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 100, 0));

        m_Texture->Bind();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind(); // Bind the shader so that we can pass uniforms
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();  // Bind the shader so that we can pass uniforms
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }
	}

	void TestTexture2D::OnImGuiRender()
	{
        ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}




}