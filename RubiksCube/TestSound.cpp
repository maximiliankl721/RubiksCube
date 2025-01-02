#include "TestSound.h"
#include <windows.h>
#include <GLFW/glfw3.h>

void TestSound::Initialze(GLFWwindow* window) {

	m_input.SetWindow(window);
	m_input.ObserveKey(GLFW_KEY_SPACE);
}

void TestSound::Update(double deltatime) {

	m_input.Update();
	if (m_input.WasKeyPressed(GLFW_KEY_SPACE)) {
		PlaySound(TEXT(""), NULL, SND_FILENAME);
	}
}

