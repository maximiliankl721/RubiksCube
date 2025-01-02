#pragma once

#include "GameInterface.h"
#include <glm/ext/quaternion_float.hpp>

class TestGlm: public GameInterface
{
public:
	virtual void Initialize();

	virtual void Update(double deltaTime);

private:
	glm::quat m_orientationQuaternion;
};

