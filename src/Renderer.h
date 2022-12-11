#pragma once

#include <GL/glew.h>
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x;ASSERT(GLLogCall(#x, __FILE__, __LINE__))
// Open gl docs
// https://docs.gl/

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	
};