#include "TestKey.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

void TestKey::Initialize(GLFWwindow* window)
{
	m_input.SetWindow(window);
	m_input.ObserveKey(GLFW_KEY_SPACE);
	m_input.ObserveKey(GLFW_KEY_RIGHT);
	m_input.ObserveKey(GLFW_KEY_LEFT);
	m_input.ObserveKey(GLFW_KEY_UP);
	m_input.ObserveKey(GLFW_KEY_DOWN);

	m_cubieRenderer.Initialize();
	m_orientationQuaternion = glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
}

void TestKey::Render(float aspectRatio)
{
	glm::mat4 globalTransformation = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f) *
		glm::lookAt(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::mat4_cast(m_orientationQuaternion);



	m_cubieRenderer.Render(globalTransformation);

}

void TestKey::ClearResources()
{
	m_cubieRenderer.ClearResources();
}

void TestKey::Update(double deltaTime)
{

	m_input.Update();
	if (m_input.WasKeyPressed(GLFW_KEY_SPACE))
		m_orientationQuaternion = glm::quat(1.0f, glm::vec3(0.0f, 0.0f ,0.0f));

	float xVel = 0.0f;
	if (m_input.IsKeyDown(GLFW_KEY_UP))
		xVel = glm::radians(90.0f);
	if (m_input.IsKeyDown(GLFW_KEY_DOWN))
		xVel = glm::radians(-90.0f);

	float yVel = 0.0f;
	if (m_input.IsKeyDown(GLFW_KEY_RIGHT))
		yVel = glm::radians(90.0f);
	if (m_input.IsKeyDown(GLFW_KEY_LEFT))
		yVel = glm::radians(-90.0f);

	glm::quat velocityQuaternion = glm::quat(0.0f, glm::vec3(xVel, yVel, 0.0f));

	m_orientationQuaternion += 0.5f * ((float)deltaTime) * velocityQuaternion * m_orientationQuaternion;
	m_orientationQuaternion = normalize(m_orientationQuaternion);
}
