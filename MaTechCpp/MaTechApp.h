#pragma once
#include "stdafx.h"

using namespace std;
using namespace sf;

class MaTechApp
{
private:
	unique_ptr<Window> mMainWindow;		// SFML main window object

public:
	MaTechApp(String title, int width, int height);
	~MaTechApp();

	void Run();
	void Render();

	void AddObject();
};

