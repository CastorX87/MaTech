#pragma once
#include "stdafx.h"
#include "Scene.h"
#include "BaseScenario.h"

using namespace std;
using namespace sf;

class MaTechApp
{
private:
	RenderWindow* mMainWindow;		// SFML main window object
	Sprite* mMainSprite;
	RenderTexture* mRenderTexture;
	BaseScenario* mScenarioPtr;

public:
	MaTechApp(String title, const Vector2i& viewportRes);
	~MaTechApp();

	void Run();

	void Render();

	inline const BaseScenario* GetScenario() const { return mScenarioPtr; }
	inline BaseScenario* GetScenario() { return mScenarioPtr; }
	inline void SetScenario(BaseScenario* scenario) { mScenarioPtr = scenario; }
};

