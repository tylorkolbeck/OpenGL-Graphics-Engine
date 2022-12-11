#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "Shader.h"

#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"


int main(void)
{
    //std::cout << "WINDOW WIDTH" << W_WIDTH << '\n';
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        // Triangle points
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

        VertexArray vao; // Create Vertex Array Buffer
        VertexBuffer vbo(positions, 4 * 4 * sizeof(float)); // Create vertex buffer
        VertexBufferLayout attribLayout; // create the attribute pointer buffer
        attribLayout.Push<float>(2);  // add pointers to attribute buffer
        attribLayout.Push<float>(2);  // add texture coordinates

        vao.AddBuffer(vbo, attribLayout); // add the vertex buffer and attribute layout to he vertex attribute array

        IndexBuffer ib(indices, 6); // set the indices

        // Setup shader program
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        // Set uniforms after a shader is bound
        //shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        // Projection Matrix - pixel space
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        
        // View Matrix - moving everything left 100 pixels
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));


        // Setup texture
        Texture texture("res/textures/xmas_text_1.png");
        texture.Bind();

        shader.SetUniform1i("u_Texture", 0);

        // Clear buffers
        vao.UnBind();
        vbo.UnBind();
        ib.UnBind();
        shader.UnBind();
        Renderer renderer;

        // Initialize IMGUI
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        // Translation
        glm::vec3 translation(200, 200, 0);


        /* Loop until the user closes the window */
        float r = 0.0f;
        float increment = 0.05f;
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();
            // Model matrix
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
            glm::mat4 mvp = proj * view * model;

            shader.Bind(); // Bind the shader so that we can pass uniforms

            //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(vao, ib, shader);


            GLClearError();

            // Increment red color for rotating rgb value on each render
            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;
            r += increment;

            {
                ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);      
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }


    // Cleanup IMGUI
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}