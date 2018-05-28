// MaTechCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MaTechApp.h"
#include "MenuScenario.h"

int main()
{
	auto menu = unique_ptr<MenuScenario>(new MenuScenario());
	auto app = unique_ptr<MaTechApp>(new MaTechApp("Hello", 640, 480));
	app->SetScenario(menu.get());
	

	app->Run();
	menu.reset();
	app.reset();

	return 0;
}

