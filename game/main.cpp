
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




// #include <glad.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

// #include <GLFW/glfw3.h>

// #include "graphics/VertexBuffer.h"
// #include "graphics/IndexBuffer.h"
// #include "graphics/VertexArray.h"
// #include "graphics/Shader.h"
// #include "graphics/Renderer.h"

// #include "engine.h"
// #include "window.h"

// #include <iostream>
// #include <cmath>
// #include <cstdlib>
// #include <ctime>
// #include <algorithm>


// ============================================================
// WINDOW
// ============================================================

const unsigned int SCR_WIDTH  = 640;
const unsigned int SCR_HEIGHT = 640;


// ============================================================
// PLAY AREA
// ============================================================

const float BOARD_LEFT   = -320.0f;
const float BOARD_RIGHT  =  320.0f;
const float BOARD_BOTTOM = -320.0f;
const float BOARD_TOP    =  320.0f;


// ============================================================
// PADDLE
// ============================================================

struct Paddle
{
    glm::vec2 position;
    glm::vec2 size;
    float speed;
};


// ============================================================
// BALL
// ============================================================

struct Ball
{
    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 velocity;
    float speed;
};


// ============================================================
// GLOBAL GAME STATE
// ============================================================

Paddle player1;
Paddle player2;
Ball ball;

int player1Score = 0;
int player2Score = 0;


// ============================================================
// INITIALIZE GAME
// ============================================================

void InitializeGame()
{
    // --------------------------------------------------------
    // Player 1
    // --------------------------------------------------------

    player1.position = glm::vec2(-280.0f, 0.0f);
    player1.size     = glm::vec2(20.0f, 100.0f);
    player1.speed    = 400.0f;


    // --------------------------------------------------------
    // Player 2
    // --------------------------------------------------------

    player2.position = glm::vec2(280.0f, 0.0f);
    player2.size     = glm::vec2(20.0f, 100.0f);
    player2.speed    = 400.0f;


    // --------------------------------------------------------
    // Ball
    // --------------------------------------------------------

    ball.position = glm::vec2(0.0f, 0.0f);
    ball.size     = glm::vec2(20.0f, 20.0f);
    ball.speed    = 350.0f;

    ball.velocity = glm::vec2(1.0f, 0.5f);

    // Normalize velocity so diagonal movement
    // doesn't accidentally become faster.

    ball.velocity = glm::normalize(ball.velocity);
    ball.velocity *= ball.speed;


    player1Score = 0;
    player2Score = 0;
}


// ============================================================
// RESET BALL
// ============================================================

void ResetBall(int direction)
{
    ball.position = glm::vec2(0.0f, 0.0f);

    float yDirection =
        (std::rand() % 2 == 0)
        ? 1.0f
        : -1.0f;

    ball.velocity = glm::vec2(
        static_cast<float>(direction),
        yDirection * 0.5f
    );

    ball.velocity = glm::normalize(ball.velocity);
    ball.velocity *= ball.speed;
}


// ============================================================
// PADDLE BOUNDARY
// ============================================================

void ClampPaddle(Paddle& paddle)
{
    float halfHeight = paddle.size.y * 0.5f;


    // Top

    if (paddle.position.y + halfHeight > BOARD_TOP)
    {
        paddle.position.y =
            BOARD_TOP - halfHeight;
    }


    // Bottom

    if (paddle.position.y - halfHeight < BOARD_BOTTOM)
    {
        paddle.position.y =
            BOARD_BOTTOM + halfHeight;
    }
}


// ============================================================
// BALL / PADDLE COLLISION
// ============================================================

bool CheckCollision(
    const Ball& ball,
    const Paddle& paddle)
{
    float ballLeft =
        ball.position.x - ball.size.x * 0.5f;

    float ballRight =
        ball.position.x + ball.size.x * 0.5f;

    float ballBottom =
        ball.position.y - ball.size.y * 0.5f;

    float ballTop =
        ball.position.y + ball.size.y * 0.5f;


    float paddleLeft =
        paddle.position.x - paddle.size.x * 0.5f;

    float paddleRight =
        paddle.position.x + paddle.size.x * 0.5f;

    float paddleBottom =
        paddle.position.y - paddle.size.y * 0.5f;

    float paddleTop =
        paddle.position.y + paddle.size.y * 0.5f;


    return
        ballRight > paddleLeft &&
        ballLeft < paddleRight &&
        ballTop > paddleBottom &&
        ballBottom < paddleTop;
}


// ============================================================
// HANDLE INPUT
// ============================================================

void HandleInput(float deltaTime)
{
    // --------------------------------------------------------
    // PLAYER 1
    // --------------------------------------------------------

    if (Input::IsKeyPressed(GLFW_KEY_W))
    {
        player1.position.y +=
            player1.speed * deltaTime;
    }

    if (Input::IsKeyPressed(GLFW_KEY_S))
    {
        player1.position.y -=
            player1.speed * deltaTime;
    }


    // --------------------------------------------------------
    // PLAYER 2
    // --------------------------------------------------------

    if (Input::IsKeyPressed(GLFW_KEY_UP))
    {
        player2.position.y +=
            player2.speed * deltaTime;
    }

    if (Input::IsKeyPressed(GLFW_KEY_DOWN))
    {
        player2.position.y -=
            player2.speed * deltaTime;
    }


    // --------------------------------------------------------
    // Clamp paddles to the screen
    // --------------------------------------------------------

    ClampPaddle(player1);
    ClampPaddle(player2);
}


// ============================================================
// HANDLE BALL / WALL COLLISION
// ============================================================

void HandleWallCollision()
{
    float halfHeight = ball.size.y * 0.5f;


    // --------------------------------------------------------
    // Top
    // --------------------------------------------------------

    if (ball.position.y + halfHeight >= BOARD_TOP)
    {
        ball.position.y =
            BOARD_TOP - halfHeight;

        ball.velocity.y =
            -std::abs(ball.velocity.y);
    }


    // --------------------------------------------------------
    // Bottom
    // --------------------------------------------------------

    if (ball.position.y - halfHeight <= BOARD_BOTTOM)
    {
        ball.position.y =
            BOARD_BOTTOM + halfHeight;

        ball.velocity.y =
            std::abs(ball.velocity.y);
    }
}


// ============================================================
// HANDLE PADDLE COLLISION
// ============================================================

void HandlePaddleCollision()
{
    // --------------------------------------------------------
    // Player 1
    // --------------------------------------------------------

    if (ball.velocity.x < 0.0f &&
        CheckCollision(ball, player1))
    {
        // Move ball outside paddle

        ball.position.x =
            player1.position.x +
            player1.size.x * 0.5f +
            ball.size.x * 0.5f;


        // Bounce right

        ball.velocity.x =
            std::abs(ball.velocity.x);


        // Change angle depending on where
        // the ball hit the paddle.

        float difference =
            ball.position.y -
            player1.position.y;

        float normalized =
            difference /
            (player1.size.y * 0.5f);

        ball.velocity.y =
            normalized * ball.speed;


        // Normalize again

        ball.velocity =
            glm::normalize(ball.velocity);

        ball.velocity *= ball.speed;
    }


    // --------------------------------------------------------
    // Player 2
    // --------------------------------------------------------

    if (ball.velocity.x > 0.0f &&
        CheckCollision(ball, player2))
    {
        // Move ball outside paddle

        ball.position.x =
            player2.position.x -
            player2.size.x * 0.5f -
            ball.size.x * 0.5f;


        // Bounce left

        ball.velocity.x =
            -std::abs(ball.velocity.x);


        // Change angle

        float difference =
            ball.position.y -
            player2.position.y;

        float normalized =
            difference /
            (player2.size.y * 0.5f);

        ball.velocity.y =
            normalized * ball.speed;


        // Normalize

        ball.velocity =
            glm::normalize(ball.velocity);

        ball.velocity *= ball.speed;
    }
}


// ============================================================
// HANDLE SCORING
// ============================================================

void HandleScoring()
{
    // --------------------------------------------------------
    // Player 2 scores
    // Ball went past player 1
    // --------------------------------------------------------

    if (ball.position.x < BOARD_LEFT)
    {
        player2Score++;

        std::cout
            << "Player 2 scored! "
            << player1Score
            << " - "
            << player2Score
            << std::endl;

        ResetBall(1);
    }


    // --------------------------------------------------------
    // Player 1 scores
    // Ball went past player 2
    // --------------------------------------------------------

    if (ball.position.x > BOARD_RIGHT)
    {
        player1Score++;

        std::cout
            << "Player 1 scored! "
            << player1Score
            << " - "
            << player2Score
            << std::endl;

        ResetBall(-1);
    }
}


// ============================================================
// UPDATE GAME
// ============================================================

void UpdateGame(float deltaTime)
{
    // --------------------------------------------------------
    // Input
    // --------------------------------------------------------

    HandleInput(deltaTime);


    // --------------------------------------------------------
    // Move ball
    // --------------------------------------------------------

    ball.position +=
        ball.velocity * deltaTime;


    // --------------------------------------------------------
    // Collision
    // --------------------------------------------------------

    HandleWallCollision();

    HandlePaddleCollision();


    // --------------------------------------------------------
    // Scoring
    // --------------------------------------------------------

    HandleScoring();
}


// ============================================================
// SET QUAD
// ============================================================
//
// vertices contains:
//
// Player 1 = vertices 0 - 3
// Player 2 = vertices 4 - 7
// Ball      = vertices 8 - 11
//
// ============================================================

void SetQuad(
    glm::vec2* vertices,
    int quadIndex,
    glm::vec2 position,
    glm::vec2 size)
{
    float halfWidth =
        size.x * 0.5f;

    float halfHeight =
        size.y * 0.5f;


    int firstVertex =
        quadIndex * 4;


    // Bottom-left

    vertices[firstVertex + 0] =
    {
        position.x - halfWidth,
        position.y - halfHeight
    };


    // Bottom-right

    vertices[firstVertex + 1] =
    {
        position.x + halfWidth,
        position.y - halfHeight
    };


    // Top-right

    vertices[firstVertex + 2] =
    {
        position.x + halfWidth,
        position.y + halfHeight
    };


    // Top-left

    vertices[firstVertex + 3] =
    {
        position.x - halfWidth,
        position.y + halfHeight
    };
}


// ============================================================
// UPDATE VERTEX BUFFER
// ============================================================

void UpdateVertexBuffer(
    VertexBuffer& vb,
    glm::vec2* vertices)
{
    vb.Bind();

    glBufferSubData(
        GL_ARRAY_BUFFER,
        0,
        12 * sizeof(glm::vec2),
        vertices
    );
}


// ============================================================
// MAIN
// ============================================================

int main()
{
    // ========================================================
    // ENGINE
    // ========================================================

    Engine engine;

    if (!engine.Init())
        return -1;


    // ========================================================
    // WINDOW
    // ========================================================

    Window window(
        SCR_WIDTH,
        SCR_HEIGHT,
        "Pong"
    );

    if (!window.Create())
        return -1;


    Input::Init(
        window.GetNativeWindow()
    );


    // ========================================================
    // OPENGL SETTINGS
    // ========================================================

    glEnable(GL_BLEND);

    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );

    glDisable(GL_DEPTH_TEST);


    // ========================================================
    // INITIALIZE GAME
    // ========================================================

    std::srand(
        static_cast<unsigned int>(
            std::time(nullptr)
        )
    );

    InitializeGame();


    // ========================================================
    // VERTICES
    // ========================================================
    //
    // We have 3 quads:
    //
    // Quad 0 = Player 1
    // Quad 1 = Player 2
    // Quad 2 = Ball
    //
    // Each quad has 4 vertices.
    //
    // Total = 12 vertices.
    //
    // ========================================================

    glm::vec2 vertices[12];


    // Initialize the vertices

    SetQuad(
        vertices,
        0,
        player1.position,
        player1.size
    );

    SetQuad(
        vertices,
        1,
        player2.position,
        player2.size
    );

    SetQuad(
        vertices,
        2,
        ball.position,
        ball.size
    );


    // ========================================================
    // INDICES
    // ========================================================
    //
    // Each quad needs:
    //
    // 0,1,2
    // 2,3,0
    //
    // 3 quads = 18 indices.
    //
    // ========================================================

    unsigned int indices[] =
    {
        // Player 1

        0, 1, 2,
        2, 3, 0,


        // Player 2

        4, 5, 6,
        6, 7, 4,


        // Ball

        8, 9, 10,
        10, 11, 8
    };


    // ========================================================
    // VERTEX ARRAY
    // ========================================================

    VertexArray va;


    // ========================================================
    // VERTEX BUFFER
    // ========================================================

    VertexBuffer vb(
        vertices,
        sizeof(vertices)
    );


    // ========================================================
    // VERTEX BUFFER LAYOUT
    // ========================================================
    //
    // Every vertex contains:
    //
    // x
    // y
    //
    // Therefore:
    //
    // Push<float>(2)
    //
    // ========================================================

    VertexBufferLayout layout;

    layout.Push<float>(2);


    // Attach VBO to VAO

    va.AddBuffer(
        vb,
        layout
    );


    // ========================================================
    // INDEX BUFFER
    // ========================================================

    IndexBuffer ibo(
        indices,
        18
    );


    // ========================================================
    // PROJECTION
    // ========================================================

    glm::mat4 proj =
        glm::ortho(
            BOARD_LEFT,
            BOARD_RIGHT,
            BOARD_BOTTOM,
            BOARD_TOP,
            -1.0f,
            1.0f
        );


    // ========================================================
    // SHADER
    // ========================================================

    Shader shader(
        "assets/basic.shader"
    );


    shader.Bind();

    shader.SetUniformMat4f(
        "u_MVP",
        proj
    );


    shader.SetUniform4f(
        "u_Color",
        1.0f,
        1.0f,
        1.0f,
        1.0f
    );


    // ========================================================
    // RENDERER
    // ========================================================

    Renderer renderer;


    // ========================================================
    // DELTA TIME
    // ========================================================

    float previousTime =
        static_cast<float>(
            glfwGetTime()
        );


    // ========================================================
    // GAME LOOP
    // ========================================================

    while (!window.ShouldClose())
    {
        // ----------------------------------------------------
        // TIME
        // ----------------------------------------------------

        float currentTime =
            static_cast<float>(
                glfwGetTime()
            );

        float deltaTime =
            currentTime - previousTime;

        previousTime =
            currentTime;


        // Prevent huge movement if the game freezes
        // for a moment.

        deltaTime =
            std::min(
                deltaTime,
                0.033f
            );


        // ----------------------------------------------------
        // UPDATE GAME
        // ----------------------------------------------------

        UpdateGame(deltaTime);


        // ----------------------------------------------------
        // UPDATE VERTICES
        // ----------------------------------------------------

        SetQuad(
            vertices,
            0,
            player1.position,
            player1.size
        );

        SetQuad(
            vertices,
            1,
            player2.position,
            player2.size
        );

        SetQuad(
            vertices,
            2,
            ball.position,
            ball.size
        );


        // ----------------------------------------------------
        // SEND NEW POSITIONS TO GPU
        // ----------------------------------------------------

        UpdateVertexBuffer(
            vb,
            vertices
        );


        // ----------------------------------------------------
        // RENDER
        // ----------------------------------------------------

        renderer.Clear();

        shader.Bind();

        renderer.Draw(
            va,
            ibo,
            shader
        );


        // ----------------------------------------------------
        // WINDOW
        // ----------------------------------------------------

        window.SwapBuffers();

        window.PollEvents();
    }


    // ========================================================
    // SHUTDOWN
    // ========================================================

    engine.Shutdown();

    return 0;
}

