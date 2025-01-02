#include "TestGlm.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "ShaderUtil.h"

void TestGlm::Initialize()
{
	glm::mat4 testMat = glm::mat4(1.0f);
	//	testMat = glm::translate(testMat, glm::vec3(1.0f, -1.0f, 1.0f));
		//testMat = glm::rotate(testMat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//testMat = testMat;
		//cameratransform
		//testMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f,0.0f), glm::vec3(0.0f, 1.0f, 0.0f ));
		//testMat = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 100.0f) * testMat;

	//slice column out of matrix
	//testMat = glm::rotate(testMat, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//glm::vec3 firstColumn = testMat[0];
	//float innerProduct = glm::dot(firstColumn, firstColumn);
	//innerProduct = innerProduct;

	GLuint test = ShaderUtil::CreateShaderProgram("VSimple.glsl", "FSimple.glsl");

	m_orientationQuaternion = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

}

void TestGlm::Update(double deltaTime)
{
	glm::quat rotVelocity = glm::quat(0.0f, glm::radians(180.0f), 0.0f, 0.0f);

	glm::quat timeDerivative = 0.5f * rotVelocity * m_orientationQuaternion;

	m_orientationQuaternion += static_cast<float>(deltaTime) * timeDerivative;
	m_orientationQuaternion = normalize(m_orientationQuaternion);

	glm::mat4 rotationMatrix = glm::mat4_cast(m_orientationQuaternion);
}
