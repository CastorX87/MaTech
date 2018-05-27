#pragma once
#include "stdafx.h"
#include "Scene.h"
#include "SceneObject.h"

using namespace std;
using namespace sf;

class BaseScenario
{
	virtual void Initialize(Scene* scenePtr) = 0;
	virtual void HandleEvents(const Event& windowEvent) = 0;
};