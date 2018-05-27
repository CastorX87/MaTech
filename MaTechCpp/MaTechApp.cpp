#include "stdafx.h"
#include "MaTechApp.h"

using namespace sf;

MaTechApp::MaTechApp(String title, int width, int height)
{
	VideoMode videoMode(width, height, 24);
	ContextSettings contextSettings(32, 0, 4, 3, 0, 0, false);
	Uint32 style = Style::Default;

	mMainWindow = unique_ptr<Window>(new Window(videoMode, title, style, contextSettings));
}

MaTechApp::~MaTechApp()
{
	mMainWindow.reset();
}

void MaTechApp::Run()
{
	while (mMainWindow->isOpen())
	{
		sf::Event event;
		while (mMainWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				mMainWindow->close();
		}
	}
}

void MaTechApp::Render()
{

}