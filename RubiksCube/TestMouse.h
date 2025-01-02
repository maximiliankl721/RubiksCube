#pragma once
#include "GameInterface.h"
#include "InputSystem.h"
#include "CubieRenderer.h"

class TestMouse : public GameInterface
{
public:
	virtual void Initialize(GLFWwindow* window);
	virtual void Render(float aspectRatio);
	virtual void ClearResource();
	virtual void Update(double deltatime);

private:
	CubieRenderer m_cubieRenderer;
	InputSystem m_input;
	glm::vec3 m_cubePosition;
	glm::mat4 m_viewProject;
};


