#include "FinalCube.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
void FinalCube::Initialize(GLFWwindow* window)
{
	m_input.SetWindow(window);
	m_input.ObserveKey(GLFW_KEY_SPACE);
	m_input.ObserveKey(GLFW_KEY_RIGHT);
	m_input.ObserveKey(GLFW_KEY_LEFT);
	m_input.ObserveKey(GLFW_KEY_UP);
	m_input.ObserveKey(GLFW_KEY_DOWN);
	m_input.ObserveKey(GLFW_KEY_D);
	m_input.ObserveKey(GLFW_KEY_A);
	m_input.ObserveKey(GLFW_KEY_W);
	m_input.ObserveKey(GLFW_KEY_S);
	m_input.ObserveKey(GLFW_KEY_1);
	m_input.ObserveKey(GLFW_KEY_2);
	m_input.ObserveKey(GLFW_KEY_3);
	m_input.ObserveKey(GLFW_KEY_4);
	m_input.ObserveKey(GLFW_KEY_5);
	m_input.ObserveKey(GLFW_KEY_6);
	m_input.ObserveKey(GLFW_KEY_7);
	m_input.ObserveKey(GLFW_KEY_8);
	m_input.ObserveKey(GLFW_KEY_9);


	m_isAnimating = false;
	m_animationDuration = 1.25f;
	m_animationTime = 0.0f;

	m_animationTimeStep = 0.0f;

	m_cubieRenderer.Initialize();
	m_orientationQuaternion = glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

	m_buffer = 0;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 3; ++k) {
					m_rotationState[0][i][j][k] = glm::mat4(1.0f);
			}
		}
	}
	
}

void FinalCube::Render(float aspectRatio)
{

	m_globalTransformation = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f) *
		glm::lookAt(glm::vec3(0.0f, 0.0f, -9.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::mat4_cast(m_orientationQuaternion);


	float offset = m_cubieRenderer.GetCubieExtension(); //+0.01f;


	if (m_isAnimating) {

		AnimateRotation();
		
	}
	else {

		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {

				for (int k = 0; k < 3; ++k) {
					glm::vec3 localPoint = glm::vec3((i - 1), (j - 1), (k - 1) );

					glm::mat4 compound = glm::mat4(1.0f);
					compound *= m_rotationState[m_buffer][i][j][k];
					
					glm::mat4 translationMatrix = glm::translate(m_globalTransformation, localPoint);
					compound = translationMatrix * compound;
					m_cubieRenderer.Render(compound);
					
				}


			}
		}
	}

}

void FinalCube::ClearResources()
{
	m_cubieRenderer.ClearResources();
}

void FinalCube::Update(double deltaTime)
{
	m_input.Update();
	UpdateSelectedKey();
	if (m_input.WasKeyPressed(GLFW_KEY_SPACE))
		m_orientationQuaternion = glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

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

	if (m_isAnimating) {
		m_animationTime += static_cast<float>(deltaTime);
		//m_animationTimeStep = glm::clamp(m_animationTime / m_animationDuration, 0.0f, 1.0f);
		m_animationTimeStep = m_animationTime / m_animationDuration;
		if (m_animationTimeStep >= 1.0f) {
			m_isAnimating = false;
			m_animationTime = 0.0f;
		}
		return;
	}

	UpdateRotation(deltaTime);

}



glm::mat4 FinalCube::GetRotationMatrix(std::string& axis, int direction)
{

	if (axis == "x") {

		return glm::rotate(glm::mat4(1.0f), glm::radians(90.f) * direction, glm::vec3(1.0f, 0.0f, 0.0f));

	}

	if (axis == "y") {

		return glm::rotate(glm::mat4(1.0f), glm::radians(90.f) * direction, glm::vec3(0.0f, 1.0f, 0.0f));

	}

	if (axis == "z") {

		return glm::rotate(glm::mat4(1.0f), glm::radians(90.f) * direction, glm::vec3(0.0f, 0.0f, 1.0f));

	}
}

void FinalCube::ChangeRotation(std::string& axis, int direction, int slice){

	glm::mat4 rotMat = GetRotationMatrix(axis, direction);

	//swap buffers
	int oldBuffer = m_buffer;
	//m_buffer = (m_buffer == 0) ? 1 : 0;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 3; ++k) {

				glm::vec3 localPoint = glm::vec3((i - 1), (j - 1), (k - 1));
				if ((j==2 && axis == "y" && slice == 1)|| (j == 1 && axis == "y" && slice == 0)|| (j == 0 && axis == "y" && slice == -1) ||
					(i == 2 && axis == "x" && slice == -1)|| (i == 1 && axis == "x" && slice == 0) || (i == 0 && axis == "x" && slice == 1) ||
					(k == 0 && axis == "z" && slice == 1) || (k == 1 && axis == "z" && slice == 0) || (k == 2 && axis == "z" && slice == -1)) {
					localPoint = static_cast<glm::mat3>(rotMat) * localPoint;

					//calculate new indices to store new the rotationmatrix in the swapped buffer
					int x = std::round(localPoint.x);
					int y = std::round(localPoint.y);
					int z = std::round(localPoint.z);

					m_rotationState[(m_buffer == 0) ? 1 : 0][x + 1][y + 1][z + 1] = m_rotationState[m_buffer][i][j][k] * rotMat;
				}
				else {
					m_rotationState[(m_buffer == 0) ? 1 : 0][i][j][k] = m_rotationState[m_buffer][i][j][k];
				}

			}
		}
	}
	//swap buffers
	m_buffer = (m_buffer == 0) ? 1 : 0;
	
}

void FinalCube::AnimateRotation()
{

	glm::mat4 rotMat = GetRotationMatrix(m_animationAxis, m_animationDirection);

	float offset = m_cubieRenderer.GetCubieExtension();
	int oldBuffer = (m_buffer == 0) ? 1 : 0;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 3; ++k) {

				glm::vec3 localPoint = glm::vec3((i - 1), (j - 1), (k - 1));
				glm::mat4 compound = glm::translate(m_globalTransformation, localPoint) * m_rotationState[oldBuffer][i][j][k];
				if ((j == 2 && m_animationAxis == "y" && m_animationSlice == 1) || (j == 1 && m_animationAxis == "y" && m_animationSlice == 0) || (j == 0 && m_animationAxis == "y" && m_animationSlice == -1) ||
					(i == 2 && m_animationAxis == "x" && m_animationSlice == -1) || (i == 1 && m_animationAxis == "x" && m_animationSlice == 0) || (i == 0 && m_animationAxis == "x" && m_animationSlice == 1) ||
					(k == 0 && m_animationAxis == "z" && m_animationSlice == 1) || (k == 1 && m_animationAxis == "z" && m_animationSlice == 0) || (k == 2 && m_animationAxis == "z" && m_animationSlice == -1)) {

					//interpolate between the identity matrix and rotation matrix according to the given step
					glm::quat rotQuat = glm::slerp(glm::quat_cast(glm::mat4(1.0f)), glm::quat_cast(rotMat), m_animationTimeStep);
					//rotQuat = glm::normalize(rotQuat); //normalize for nlerp

					auto finalMat = glm::mat4_cast(rotQuat) * m_rotationState[oldBuffer][i][j][k];

					localPoint = glm::mat3_cast(rotQuat) * localPoint;

					compound = glm::translate(m_globalTransformation, localPoint) *finalMat;
					
					
				}
				m_cubieRenderer.Render(compound);
			}
		}
	}
	
}

void FinalCube::SetAnimationProperties(std::string& axis, int direction, int slice)
{
	m_isAnimating = true;
	m_animationAxis = axis;
	m_animationDirection = direction;
	m_animationSlice = slice;
}

void FinalCube::UpdateRotation(double deltaTime)
{
	auto rotMatWholeCube = glm::mat4_cast(m_orientationQuaternion);

	float colVecXx = rotMatWholeCube[0].x;
	float colVecYx = rotMatWholeCube[1].x;
	float colVecZx = rotMatWholeCube[2].x;

	//auto test = rotMatWholeCube[0];

	auto maxX = std::max({ abs(colVecXx), abs(colVecYx), abs(colVecZx) });

	float colVecXy = rotMatWholeCube[0].y;
	float colVecYy = rotMatWholeCube[1].y;
	float colVecZy = rotMatWholeCube[2].y;

	auto maxY = std::max({ abs(colVecXy), abs(colVecYy), abs(colVecZy) });

	float colVecXz = rotMatWholeCube[0].z;
	float colVecYz = rotMatWholeCube[1].z;
	float colVecZz = rotMatWholeCube[2].z;

	auto maxZ = std::max({ abs(colVecXz), abs(colVecYz), abs(colVecZz) });

	/*
		layout of cube slices and indices
			x-axis slices:
				-1, left -> in for loop i = 2
				0, middle -> in for loop i = 1
				1, right -> in for loop i = 0
			y-axis slices:
				1, up  -> in for loop j = 2
				0, middle -> in for loop j = 1
				-1, down -> in for loop j = 0
			z-axis slices:
				1, front  -> in for loop k = 0
				0, middle  -> in for loop k = 1
				-1, back  -> in for loop k = 2
				
	*/

	if (m_input.WasKeyPressed(GLFW_KEY_D)) {

		
		switch (selected_key) {
		case 1:

			if (maxY == abs(colVecXy)) {

				int slice = (colVecXy >= 0) ? -1 : 1;
				int direction = (slice == 1) ? -1 : 1; // direction 3 == 270 degree == -90 degree == -1, using -1 for NLERP runs into gimbal lock in animation 
				std::string axis = "x";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}
			else if (maxY == abs(colVecYy)) {

				int slice = (colVecYy >= 0) ? 1 : -1;
				int direction = (slice == 1) ? 1 : -1;
				std::string axis = "y";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}
			else if (maxY == abs(colVecZy)) {

				int slice = (colVecZy >= 0) ? -1 : 1;
				int direction = (slice == 1) ? -1 : 1;
				std::string axis = "z";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}

			break;

		case 2:

			if (maxY == abs(colVecXy)) {

				int direction = (colVecXy >= 0) ? 1 : -1;
				std::string axis = "x";

				SetAnimationProperties(axis, direction, 0);
				ChangeRotation(axis, direction, 0);

			}
			else if (maxY == abs(colVecYy)) {

				int direction = (colVecYy >= 0) ? 1 : -1;
				std::string axis = "y";

				SetAnimationProperties(axis, direction, 0);
				ChangeRotation(axis, direction, 0);

			}
			else if (maxY == abs(colVecZy)) {

				int direction = (colVecZy >= 0) ? 1 : -1;
				std::string axis = "z";

				SetAnimationProperties(axis, direction, 0);
				ChangeRotation(axis, direction, 0);

			}

			break;

		case 3:
			if (maxY == abs(colVecXy)) {

				int slice = (colVecXy >= 0) ? 1 : -1;
				int direction = (slice == 1) ? 1 : -1;
				std::string axis = "x";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}
			else if (maxY == abs(colVecYy)) {

				int slice = (colVecYy >= 0) ? -1 : 1;
				int direction = (slice == 1) ? -1 : 1;
				std::string axis = "y";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}
			else if (maxY == abs(colVecZy)) {

				int slice = (colVecZy >= 0) ? 1 : -1;
				int direction = (slice == 1) ? 1 : -1;
				std::string axis = "z";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}

			break;

		case 7:

			if (maxZ == abs(colVecXz)) {

				int slice = (colVecXz >= 0) ? 1 : -1;
				int direction = (slice == 1) ? 1 : -1;
				std::string axis = "x";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}
			else if (maxZ == abs(colVecYz)) {

				int slice = (colVecYz >= 0) ? -1 : 1;
				int direction = (slice == 1) ? -1 : 1;
				std::string axis = "y";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}
			else if (maxZ == abs(colVecZz)) {

				int slice = (colVecZz >= 0) ? 1 : -1;
				int direction = (slice == 1) ? 1 : -1;
				std::string axis = "z";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}

			break;

		case 8:
			if (maxZ == abs(colVecXz)) {

				int direction = (colVecXz >= 0) ? 1 : -1;
				std::string axis = "x";

				SetAnimationProperties(axis, direction, 0);
				ChangeRotation(axis, direction, 0);

			}
			else if (maxZ == abs(colVecYz)) {

				int direction = (colVecYz >= 0) ? 1 : -1;
				std::string axis = "y";

				SetAnimationProperties(axis, direction, 0);
				ChangeRotation(axis, direction, 0);

			}
			else if (maxZ == abs(colVecZz)) {

				int direction = (colVecZz >= 0) ? 1 : -1;
				std::string axis = "z";

				SetAnimationProperties(axis, direction, 0);
				ChangeRotation(axis, direction, 0);

			}

			break;
		case 9:
			if (maxZ == abs(colVecXz)) {

				int slice = (colVecXz >= 0) ? -1 : 1;
				int direction = (slice == 1) ? -1 : 1;
				std::string axis = "x";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}
			else if (maxZ == abs(colVecYz)) {

				int slice = (colVecYz >= 0) ? 1 : -1;
				int direction = (slice == 1) ? 1 : -1;
				std::string axis = "y";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}
			else if (maxZ == abs(colVecZz)) {

				int slice = (colVecZz >= 0) ? -1 : 1;
				int direction = (slice == 1) ? -1 : 1;
				std::string axis = "z";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}
			break;
		}

	}

	if (m_input.WasKeyPressed(GLFW_KEY_A)) {
		

		switch (selected_key) {

		case 1:

			if (maxY == abs(colVecXy)) {

				int slice = (colVecXy >= 0) ? -1 : 1;
				int direction = (slice == 1) ? 1 : 3;
				std::string axis = "x";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}
			else if (maxY == abs(colVecYy)) {

				int slice = (colVecYy >= 0) ? 1 : -1;
				int direction = (slice == 1) ? 3 : 1;
				std::string axis = "y";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}
			else if (maxY == abs(colVecZy)) {

				int slice = (colVecZy >= 0) ? -1 : 1;
				int direction = (slice == 1) ? 1 : 3;
				std::string axis = "z";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}

			break;

		case 2:

			if (maxY == abs(colVecXy)) {

				int direction = (colVecXy >= 0) ? 3 : 1;
				std::string axis = "x";

				SetAnimationProperties(axis, direction, 0);
				ChangeRotation(axis, direction, 0);

			}
			else if (maxY == abs(colVecYy)) {

				int direction = (colVecYy >= 0) ? 3 : 1;
				std::string axis = "y";

				SetAnimationProperties(axis, direction, 0);
				ChangeRotation(axis, direction, 0);

			}
			else if (maxY == abs(colVecZy)) {

				int direction = (colVecZy >= 0) ? 3 : 1;
				std::string axis = "z";

				SetAnimationProperties(axis, direction, 0);
				ChangeRotation(axis, direction, 0);

			}

			break;

		case 3:
			if (maxY == abs(colVecXy)) {

				int slice = (colVecXy >= 0) ? 1 : -1;
				int direction = (slice == 1) ? 3 : 1;
				std::string axis = "x";
				
				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}
			else if (maxY == abs(colVecYy)) {

				int slice = (colVecYy >= 0) ? -1 : 1;
				int direction = (slice == 1) ? 1 : 3;
				std::string axis = "y";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}
			else if (maxY == abs(colVecZy)) {

				int slice = (colVecZy >= 0) ? 1 : -1;
				int direction = (slice == 1) ? 3 : 1;
				std::string axis = "z";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}

			break;

		case 7:
			if (maxZ == abs(colVecXz)) {

				int slice = (colVecXz >= 0) ? 1 : -1;
				int direction = (slice == 1) ? 3 : 1;
				std::string axis = "x";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}
			else if (maxZ == abs(colVecYz)) {

				int slice = (colVecYz >= 0) ? -1 : 1;
				int direction = (slice == 1) ? 1 : 3;
				std::string axis = "y";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}
			else if (maxZ == abs(colVecZz)) {

				int slice = (colVecZz >= 0) ? 1 : -1;
				int direction = (slice == 1) ? 3 : 1;
				std::string axis = "z";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}

			break;

		case 8:
			if (maxZ == abs(colVecXz)) {

				int direction = (colVecXz >= 0) ? 3 : 1;
				std::string axis = "x";

				SetAnimationProperties(axis, direction, 0);
				ChangeRotation(axis, direction, 0);

			}
			else if (maxZ == abs(colVecYz)) {

				int direction = (colVecYz >= 0) ? 3 : 1;
				std::string axis = "y";

				SetAnimationProperties(axis, direction, 0);
				ChangeRotation(axis, direction, 0);

			}
			else if (maxZ == abs(colVecZz)) {

				int direction = (colVecZz >= 0) ? 3 : 1;
				std::string axis = "z";

				SetAnimationProperties(axis, direction, 0);
				ChangeRotation(axis, direction, 0);

			}

			break;

		case 9:
			if (maxZ == abs(colVecXz)) {

				int slice = (colVecXz >= 0) ? -1 : 1;
				int direction = (slice == 1) ? 1 : 3;
				std::string axis = "x";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}
			else if (maxZ == abs(colVecYz)) {

				int slice = (colVecYz >= 0) ? 1 : -1;
				int direction = (slice == 1) ? 3 : 1;
				std::string axis = "y";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}
			else if (maxZ == abs(colVecZz)) {

				int slice = (colVecZz >= 0) ? -1 : 1;
				int direction = (slice == 1) ? 1 : 3;
				std::string axis = "z";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);

			}
			break;
		}
	}

	if (m_input.WasKeyPressed(GLFW_KEY_W)) {

		switch (selected_key) {
		case 4:

			if (maxX == abs(colVecXx)) {

				int slice = (colVecXx >= 0) ? -1 : 1;
				int direction = (slice == 1) ? 3 : 1;
				std::string axis = "x";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);
			}
			else if (maxX == abs(colVecYx)) {

				int slice = (colVecYx >= 0) ? 1 : -1;
				int direction = (slice == 1) ? 1 : 3;
				std::string axis = "y";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);
			}
			else if (maxX == abs(colVecZx)) {

				int slice = (colVecZx >= 0) ? -1 : 1;
				int direction = (slice == 1) ? 3 : 1;
				std::string axis = "z";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);
			}

			break;

		case 5:
			if (maxX == abs(colVecXx)) {


				int direction = (colVecXx >= 0) ? 1 : 3;
				std::string axis = "x";

				SetAnimationProperties(axis, direction, 0);
				ChangeRotation(axis, direction, 0);
			}
			else if (maxX == abs(colVecYx)) {

				int direction = (colVecYx >= 0) ? 1 : 3;
				std::string axis = "y";

				SetAnimationProperties(axis, direction, 0);
				ChangeRotation(axis, direction, 0);
			}
			else if (maxX == abs(colVecZx)) {


				int direction = (colVecZx >= 0) ? 1 : 3;
				std::string axis = "z";

				SetAnimationProperties(axis, direction, 0);
				ChangeRotation(axis, direction, 0);
			}

			break;

		case 6:

			if (maxX == abs(colVecXx)) {

				int slice = (colVecXx >= 0) ? 1 : -1;
				int direction = (slice == 1) ? 1 : 3;
				std::string axis = "x";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);
			}
			else if (maxX == abs(colVecYx)) {

				int slice = (colVecYx >= 0) ? -1 : 1;
				int direction = (slice == 1) ? 3 : 1;
				std::string axis = "y";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);
			}
			else if (maxX == abs(colVecZx)) {

				int slice = (colVecZx >= 0) ? 1 : -1;
				int direction = (slice == 1) ? 1 : 3;
				std::string axis = "z";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);
			}

			break;
		}

	}

	if (m_input.WasKeyPressed(GLFW_KEY_S)) {


		switch (selected_key) {
		case 4:

			if (maxX == abs(colVecXx)) {

				int slice = (colVecXx >= 0) ? -1 : 1;
				int direction = (slice == 1) ? 1 : 3;
				std::string axis = "x";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);
			}
			else if (maxX == abs(colVecYx)) {

				int slice = (colVecYx >= 0) ? 1 : -1;
				int direction = (slice == 1) ? 3 : 1;
				std::string axis = "y";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);
			}
			else if (maxX == abs(colVecZx)) {

				int slice = (colVecZx >= 0) ? -1 : 1;
				int direction = (slice == 1) ? 1 : 3;
				std::string axis = "z";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);
			}

			break;

		case 5:
			if (maxX == abs(colVecXx)) {


				int direction = (colVecXx >= 0) ? 3 : 1;
				std::string axis = "x";

				SetAnimationProperties(axis, direction, 0);
				ChangeRotation(axis, direction, 0);
			}
			else if (maxX == abs(colVecYx)) {

				int direction = (colVecYx >= 0) ? 3 : 1;
				std::string axis = "y";

				SetAnimationProperties(axis, direction, 0);
				ChangeRotation(axis, direction, 0);
			}
			else if (maxX == abs(colVecZx)) {


				int direction = (colVecZx >= 0) ? 3 : 1;
				std::string axis = "z";

				SetAnimationProperties(axis, direction, 0);
				ChangeRotation(axis, direction, 0);
			}

			break;

		case 6:

			if (maxX == abs(colVecXx)) {

				int slice = (colVecXx >= 0) ? 1 : -1;
				int direction = (slice == 1) ? 3 : 1;
				std::string axis = "x";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);
			}
			else if (maxX == abs(colVecYx)) {

				int slice = (colVecYx >= 0) ? -1 : 1;
				int direction = (slice == 1) ? 1 : 3;
				std::string axis = "y";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);
			}
			else if (maxX == abs(colVecZx)) {

				int slice = (colVecZx >= 0) ? 1 : -1;
				int direction = (slice == 1) ? 3 : 1;
				std::string axis = "z";

				SetAnimationProperties(axis, direction, slice);
				ChangeRotation(axis, direction, slice);
			}

			break;
		}
	}
}

void FinalCube::UpdateSelectedKey()
{
	if (m_input.WasKeyPressed(GLFW_KEY_1))
		selected_key = 1;
	if (m_input.WasKeyPressed(GLFW_KEY_2))
		selected_key = 2;
	if (m_input.WasKeyPressed(GLFW_KEY_3))
		selected_key = 3;
	if (m_input.WasKeyPressed(GLFW_KEY_4))
		selected_key = 4;
	if (m_input.WasKeyPressed(GLFW_KEY_5))
		selected_key = 5;
	if (m_input.WasKeyPressed(GLFW_KEY_6))
		selected_key = 6;
	if (m_input.WasKeyPressed(GLFW_KEY_7))
		selected_key = 7;
	if (m_input.WasKeyPressed(GLFW_KEY_8))
		selected_key = 8;
	if (m_input.WasKeyPressed(GLFW_KEY_9))
		selected_key = 9;
}