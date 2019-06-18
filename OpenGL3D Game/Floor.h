#pragma once

#include "Object.h"

class CFloor : public CObject
{
public:
	CFloor(glm::vec3 _Pos);
	void Update(float _DeltaTime);
};