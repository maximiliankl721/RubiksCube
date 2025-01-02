// RubiksCube.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//
#include "GameInterface.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "TestGlm.h"
#include "TestTriangle.h"
#include "TestCubie.h"
#include "TestCompoundCube.h"
#include "TestKey.h"
#include "TestMouse.h"
#include "TestTexture.h"
#include "TestSound.h"

#include "FinalCube.h"
#include <memory>

//Collection of test classes
GameInterface gDummyTest;
TestGlm gTestGlm;
TestTriangle gTestTriangle;
TestCubie gTestCubie;
TestCompoundCube gTestCompoundCube;
TestKey gTestKey;
TestMouse gTestMouse;
TestTexture gTestTexture;
TestSound gTestSound;

FinalCube gFinalCube;

std::unique_ptr<GameInterface> gUsedInterface;

GLFWwindow* InitializeSystem() {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1024, 768, "Project Rubiks Cube", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	gUsedInterface->Initialize(window);

	return window;

}

void RunCoreloop(GLFWwindow* window) {

	double lastTime = glfwGetTime();
	double timeDifference = 0.0f;

	while (!glfwWindowShouldClose(window)) {
	
		glfwPollEvents();

		gUsedInterface->Update(timeDifference);
		int screenWidth, screenHeight;
		glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
		float aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
		glViewport(0, 0, screenWidth, screenHeight);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gUsedInterface->Render(aspectRatio);
		glfwSwapBuffers(window);

		double currentTime = glfwGetTime();
		timeDifference = currentTime - lastTime;
		lastTime = currentTime;
	}
}

void ShutdownSystem() {
	
	gUsedInterface->ClearResources();
	glfwTerminate();
};

int main()
{
	//gUsedInterface = std::make_unique<DummyTest>(gDummyTest);
	//gUsedInterface = std::make_unique<TestGlm>(gTestGlm);
	//gUsedInterface = std::make_unique<TestTriangle>(gTestTriangle);
	//gUsedInterface = std::make_unique<TestCubie>(gTestCubie);
	//gUsedInterface = std::make_unique<TestCompoundCube>(gTestCompoundCube);
	//gUsedInterface = std::make_unique<TestKey>(gTestKey);
	//gUsedInterface = std::make_unique<TestMouse>(gTestMouse);
	//gUsedInterface = std::make_unique<TestTexture>(gTestTexture);
	//gUsedInterface = std::make_unique<TestSound>(gTestSound);

	gUsedInterface = std::make_unique<FinalCube>(gFinalCube);

	GLFWwindow* window = InitializeSystem();
	RunCoreloop(window);
	ShutdownSystem();
}

