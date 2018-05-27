#pragma once
#include "stdafx.h"
#include "Scene.h"

using namespace std;
using namespace sf;

class MaTechApp
{
private:
	RenderWindow* mMainWindow;		// SFML main window object
	Scene* mScene;
	Sprite* mMainSprite;
	RenderTexture* mRenderTexture;

public:
	MaTechApp(String title, int width, int height);
	~MaTechApp();

	void Run();
	void Render();

	inline const Scene* GetScene() const { return mScene; }
	inline Scene* GetScene() { return mScene; }
};

