#include "SimpleCubieRenderer.h"
#include "ShaderUtil.h"
#include <glm/gtc/type_ptr.hpp>

void SimpleCubieRenderer::Initialize() {

	std::vector<glm::vec3> positionField;
	std::vector<glm::vec3> colorField;



	int test = sizeof(glm::vec3);

	//Build the cube information
	for (int sideType = 0; sideType < 3; ++sideType) {
		for (int direction = -1; direction < 2; direction += 2) {
			AddSidePosition(sideType, direction, positionField);
			AddSideColor(sideType, direction, colorField);
		}
	}

	m_shaderProgram = ShaderUtil::CreateShaderProgram("VColor.glsl", "FColor.glsl");
	m_transformLocation = glGetUniformLocation(m_shaderProgram, "transformation");

	glGenVertexArrays(1, &m_arrayBufferObject);
	glGenBuffers(2, m_vertexBufferObject);

	glBindVertexArray(m_arrayBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[0]);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*positionField.size(), positionField.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, 12 * positionField.size(), positionField.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[1]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colorField.size(), colorField.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


}

void SimpleCubieRenderer::Render(const glm::mat4& transformationMatrix)const {

	glUseProgram(m_shaderProgram);
	glBindVertexArray(m_arrayBufferObject);

	glUniformMatrix4fv(m_transformLocation, 1, GL_FALSE, glm::value_ptr(transformationMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 6 * 6);

	glBindVertexArray(0);
	glUseProgram(0);
}

void SimpleCubieRenderer::ClearResources()const {
	glDeleteBuffers(2, m_vertexBufferObject);
	glDeleteVertexArrays(1, &m_arrayBufferObject);
	glDeleteProgram(m_shaderProgram);
}

void SimpleCubieRenderer::AddSidePosition(int sideType, int direction, std::vector<glm::vec3>& positionArray)const {

	glm::vec3 cornerPoints[2][2];

	int localXDim = (sideType + 1) % 3;
	int localYDim = (sideType + 2) % 3;

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			glm::vec3 localPoint(direction * m_offset);
			localPoint[localXDim] = (2 * i - 1) * m_offset;
			localPoint[localYDim] = (2 * j - 1) * m_offset;
			cornerPoints[i][j] = localPoint;
		}
	}

	positionArray.push_back(cornerPoints[0][0]);
	positionArray.push_back(cornerPoints[1][0]);
	positionArray.push_back(cornerPoints[0][1]);
	positionArray.push_back(cornerPoints[1][0]);
	positionArray.push_back(cornerPoints[0][1]);
	positionArray.push_back(cornerPoints[1][1]);

}

void SimpleCubieRenderer::AddSideColor(int sideType, int direction, std::vector<glm::vec3>& colorArray) {

	glm::vec3 color = glm::vec3(0.0f);

	float baseColor = (direction == -1) ? 0.5 : 1.0f;
	color[sideType] = baseColor;

	for (int i = 0; i < 6; ++i) {
		colorArray.push_back(color);
	}

}

