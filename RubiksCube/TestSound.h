#pragma once
#include "InputSystem.h"
#include "GameInterface.h"

class TestSound : public GameInterface
{

public:
	void Initialze(GLFWwindow* window);
	void Update(double deltatime);
private:
	InputSystem m_input;

};

