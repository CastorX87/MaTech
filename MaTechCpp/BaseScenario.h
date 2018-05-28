#pragma once
#include "stdafx.h"
#include "Scene.h"
#include "SceneObject.h"

using namespace std;
using namespace sf;

class BaseScenario
{
protected:
	Scene* mScene;

public:
	BaseScenario()
	{
		mScene = new Scene();
	};

	virtual ~BaseScenario()
	{
		SafeDelete(mScene);
	};

	Scene* GetScene()
	{
		return mScene;
	}

	const Scene* GetScene() const
	{
		return mScene;
	}

	virtual void HandleEvents(const Event& windowEvent) = 0;
};