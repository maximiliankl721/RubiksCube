#pragma once
#include "GameInterface.h"
#include "SimpleCubieRenderer.h"
#include "InputSystem.h"
#include <glm/ext/quaternion_float.hpp>
#include <string>
class FinalCube : public GameInterface {

public:
	virtual void Initialize(GLFWwindow* window);
	virtual void Render(float aspectRatio);
	virtual void ClearResources();
	virtual void Update(double deltaTime);

private:
	void SetAnimationProperties(std::string& axis, int direction, int slice);
	void UpdateRotation(double deltaTime);
	void UpdateSelectedKey();
	glm::mat4 GetRotationMatrix(std::string& axis, int direction);
	void ChangeRotation(std::string& axis, int direction, int slice);
	void AnimateRotation();

private:
	SimpleCubieRenderer m_cubieRenderer;
	InputSystem m_input;

	glm::mat4 m_rotationState[2][3][3][3];

	int m_buffer;

	glm::quat m_orientationQuaternion;
	glm::mat4 m_globalTransformation;

	int selected_key = 0;

	bool m_isAnimating;
	float m_animationTime;
	float m_animationDuration;
	float m_animationTimeStep;

	std::string m_animationAxis;
	int m_animationSlice;
	int m_animationDirection;
};

