#pragma once

#include <glad.h>
#include <cassert>


#include "graphics/VertexArray.h"
#include "graphics/IndexBuffer.h"
#include "graphics/Shader.h"

#define GLCall(x) GLClearError();\
    x;\
    assert(GLLogCall(#x, __FILE__, __LINE__)) 
    

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
private:
    /* data */
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};

