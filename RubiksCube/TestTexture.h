#pragma once
#include "GameInterface.h"
#include <GL/glew.h>

class TestTexture : public GameInterface
{

public:
	virtual void Initialize();
	virtual void Render(float aspectRatio);
	virtual void ClearResources();

private:
	GLuint m_shaderProgram;
	GLuint m_vertexBufferObject;
	GLuint m_arrayBufferObject;
	GLint m_transformLocation;
	GLuint m_texture;


};

