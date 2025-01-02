#include "TestTexture.h"
#include "ShaderUtil.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void TestTexture::Initialize() {

	m_shaderProgram = ShaderUtil::CreateShaderProgram("VTexture.glsl", "FTexture.glsl");

	const float position[] = { -1.0f, -1.0f, 0.0f,
								1.0f, -1.0f, 0.0f,
								0.0f, 1.0f,0.0f };
	m_transformLocation = glGetUniformLocation(m_shaderProgram, "transformation");

	glGenVertexArrays(1, &m_arrayBufferObject);
	glGenBuffers(1, &m_vertexBufferObject);

	glBindVertexArray(m_arrayBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	int w, h, comp;
	unsigned char* image = stbi_load("Texture.jpg", &w, &h, &comp, STBI_rgb_alpha);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(image);
}

void TestTexture::ClearResources()
{

	glDeleteProgram(m_shaderProgram);
	glDeleteBuffers(1, &m_vertexBufferObject);
	glDeleteVertexArrays(1, &m_arrayBufferObject);

	glDeleteTextures(1, &m_texture);

}


void TestTexture::Render(float aspectRatio)
{
	glUseProgram(m_shaderProgram);
	glBindVertexArray(m_arrayBufferObject);

	glm::mat4 globalTransformation = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f) *
		glm::lookAt(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(m_transformLocation, 1, GL_FALSE, glm::value_ptr(globalTransformation));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	glUseProgram(0);

}

