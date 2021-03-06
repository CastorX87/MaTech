// MaTechCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MaTechApp.h"
#include "MenuScenario.h"

int main()
{
	sf::Vector2i res(1280, 720);

	auto mainMenu = unique_ptr<MenuScenario>(new MenuScenario(res));
	auto app = unique_ptr<MaTechApp>(new MaTechApp("Hello", res));
	app->SetScenario(mainMenu.get());
	

	app->Run();
	mainMenu.reset();
	app.reset();

	return 0;
}

