#pragma once
#include "SimpleCubieRenderer.h"
#include "GameInterface.h"
#include "InputSystem.h"
#include <glm/ext/quaternion_float.hpp>
class TestKey : public GameInterface
{
public:
	virtual void Initialize(GLFWwindow* window);
	virtual void Render(float aspectRatio);
	virtual void ClearResources();
	virtual void Update(double deltaTime);

private:
	SimpleCubieRenderer m_cubieRenderer;
	InputSystem m_input;
	glm::quat m_orientationQuaternion;
};

