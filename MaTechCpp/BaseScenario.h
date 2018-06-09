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
	Scene mScene;
	Camera mCamera;

public:
	BaseScenario(const Vector2i& viewportSize)
	{
		mCamera.ViewportSize = viewportSize;
		mCamera.CenterPos = Vector2f(0, 0);
		mCamera.ZoomFactor = 1;
	};

	virtual ~BaseScenario()
	{
	};

	const Scene& GetScene() const
	{
		return mScene;
	}

	const Camera& GetCamera() const
	{
		return mCamera;
	}

	void SetCamera(const Camera& camera)
	{
		mCamera = camera;
	}

	virtual void HandleEvents(const Event& windowEvent) = 0;
};