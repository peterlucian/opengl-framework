
#include <glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

#include "graphics/VertexBuffer.h"
#include "graphics/IndexBuffer.h"
#include "graphics/VertexArray.h"
#include "graphics/Shader.h"
#include "graphics/Renderer.h"
#include "graphics/Texture.h"

#include "engine.h"
#include "window.h"



#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdlib>     /* srand, rand */
#include <ctime>
#include <queue>
#include <utility>
#include <vector>


// unsigned int load_textures(){
//     unsigned int texArrayId;
//     glGenTextures(1, &texArrayId);
//     glBindTexture(GL_TEXTURE_2D_ARRAY, texArrayId);
//     //glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 128, 128, 12);

//     // Specify the storage for the texture array
//     glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, 128, 128, 12, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

//     for (int i=0; i < 12; i++){
//         int width, height, channels;
//         stbi_set_flip_vertically_on_load(true);
//         //std::cout << filesPath[i] << std::endl;
//         unsigned char* data = stbi_load(filesPath[i], &width, &height, &channels, STBI_rgb_alpha);

//         glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, 128, 128, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

//         stbi_image_free(data);
//     }


//     glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

//     return texArrayId;
// }

int main()
{

        Engine engine;

        if (!engine.Init())
            return -1;

        Window window(640, 640, "Pong");

        if (!window.Create())
            return -1;

        // while (!window.ShouldClose())
        // {
        //     window.SwapBuffers();
        //     window.PollEvents();
        // }
        Input::Init(window.GetNativeWindow());


        //window.SetMouseButtonCallback(mouse_button_callback);
        //window.SetFramebufferSizeCallback(framebuffer_size_callback);


        // placeMemes(10);
        // calculateMemeCounts();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);

       
        // float vertices[] = {
        //     -250.0f,  -250.0f,   0.0f, 0.0f, // left
        //     -200.0f,  -250.0f,   1.0f, 0.0f, // right
        //     -200.0f,  -200.0f,   1.0f, 1.0f, // top
        //     -250.0f,  -200.0f,   0.0f, 1.0f
        // };


        // unsigned int indices[]= {
        //     0, 1, 2,
        //     2, 3, 0
        // };

        // glm::vec2 quadVertices[] = {
        //     {-0.0f,  -25.0f},  // Bottom-left
        //     {-0.0f,  -25.0f},  // Bottom-right
        //     {-25.0f,  -0.0f},  // Top-right
        //     {-25.0f,  -0.0f}  // Top-left
        // };

         glm::vec2 quadVertices[] =
        {
            {-10.0f, -50.0f}, // bottom-left
            { 10.0f, -50.0f}, // bottom-right
            { 10.0f,  50.0f}, // top-right
            {-10.0f,  50.0f}  // top-left
        };
        unsigned int indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };
        
        


        //Create the vertex Array and vertex Buffer
        //VertexBuffer vb (vertices, 4 * 4 * sizeof(float));
        VertexBuffer vb(&quadVertices[0], 4 * sizeof(glm::vec2) * sizeof(float));
        VertexArray va;

        // Create the vertex array layout and bind the buffer and the layout
        VertexBufferLayout layout;
        //layout.push(2, VALUETYPE::FLOAT);
        //layout.push(2, VALUETYPE::FLOAT);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);
        
        IndexBuffer ibo(indices, 6);

       

        Shader shader("assets/basic.shader");
        //shader.SetUniform4f("u_Color", 1.0f, 0.5f, 0.2f, 1.0f);
        shader.Bind();
        glm::mat4 proj = glm::ortho(
            -320.0f, 320.0f,
            -320.0f, 320.0f,
            -1.0f, 1.0f
        );

        shader.SetUniformMat4f("u_MVP", proj);
        



        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        // va.Unbind();
        // vb.Unbind();
        // shader.Unbind();
        Renderer renderer;
        // uncomment this call to draw in wireframe polygons.
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // Update the buffer using glBufferSubData
        // finalVertexBuffer[0].textureID = 5.0f;
        // GLintptr offset = 0 * sizeof(Vertex);
        // glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(Vertex), &finalVertexBuffer[0]);

        
        // render loop
        // -----------
        while (!window.ShouldClose())
        {

            bool left = Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
            bool right = Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT);

            renderer.Clear();

            shader.Bind();

            renderer.Draw(va, ibo, shader);

            window.SwapBuffers();
            window.PollEvents();
                
        }

    engine.Shutdown();
    return 0;
}



