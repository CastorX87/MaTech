#pragma once
#include "stdafx.h"

using namespace sf;

struct Camera
{

public:
	Vector2f CenterPos;				// Center point of the camera in game coordiante system
	float ZoomFactor;					// Camera zoom factor
	Vector2i ViewportSize;		// Window coordiante system
};