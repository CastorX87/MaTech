#pragma once
#include "stdafx.h"
#include "Scene.h"
#include "SceneObject.h"
#include "Camera.h"

using namespace std;
using namespace sf;

class BaseScenario
{
protected:
	Scene* mScene;
	Camera mCamera;
	b2World* mWorld;

public:
	BaseScenario(const Vector2i& viewportSize)
	{
		mScene = new Scene();
		mCamera.ViewportSize = viewportSize;
		mCamera.CenterPos = Vector2f(0, 0);
		mCamera.ZoomFactor = 0.1;
		mWorld = new b2World(b2Vec2(0, -10));
	};

	virtual ~BaseScenario()
	{
		SafeDelete(mScene);
		SafeDelete(mWorld);
	};

	const Scene* GetScene() const
	{
		return mScene;
	}

	Scene* GetScene()
	{
		return mScene;
	}

	const b2World* GetWorld() const
	{
		return mWorld;
	}

	b2World* GetWorld()
	{
		return mWorld;
	}

	const Camera& GetCamera() const
	{
		return mCamera;
	}

	void SetCamera(const Camera& camera)
	{
		mCamera = camera;
	}

	void AdvanceTime(float deltaTimeSec)
	{
		mWorld->Step(deltaTimeSec, 6, 4);
	}

	virtual void HandleEvents(const Event& windowEvent) = 0;
};