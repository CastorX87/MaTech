// MaTechCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MaTechApp.h"

int main()
{
	auto app = unique_ptr<MaTechApp>(new MaTechApp("Hello", 640, 480));

	app->GetScene()->AddObject(new Background("Resources\\Textures\\pngtree_test.png"));

	app->Run();
	app.reset();

	return 0;
}

