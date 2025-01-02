#pragma once
#include "GameInterface.h"
#include "SimpleCubieRenderer.h"

class TestCubie: public GameInterface {

public:
	virtual void Initialize();
	virtual void Render(float aspectRatio);
	virtual void ClearResources();
	virtual void Update(double deltaTime);

private:
	SimpleCubieRenderer m_cubieRenderer;
	float m_turningAngle;

};

