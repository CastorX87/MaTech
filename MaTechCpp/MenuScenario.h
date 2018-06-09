#pragma once
#include "stdafx.h"
#include "BaseScenario.h"
#include "Background.h"
#include "BaseButton.h"

class MenuScenario :
	public BaseScenario
{
private:
	Font* mFont;

public:
	MenuScenario(const Vector2i& viewportSize);
	virtual ~MenuScenario();
	virtual void HandleEvents(const Event& windowEvent);
};

int OnClick_Quit(const Event& event, void* scenario)
{
	BaseScenario* currScenario = (BaseScenario*)scenario;
	return 0;
}

MenuScenario::MenuScenario(const Vector2i& viewportSize) :
	BaseScenario(viewportSize)
{
	mFont = new Font();
	mFont->loadFromFile("Resources\\Fonts\\Amiko-Regular.ttf");
	mScene.AddObject(new Background("Resources\\Textures\\pngtree_test.png"));
	BaseButton* btnQuit = new BaseButton("Resources\\Textures\\ButtonBackgrountTest.png", L"Kilépés", *mFont, 24, 0, 0, Vector2f(0.3, 0.2), Vector2f(0.25, 0.25));
	btnQuit->RegisterOnClick(OnClick_Quit, this);
	mScene.AddObject(btnQuit);
}

MenuScenario::~MenuScenario()
{
	SafeDelete(mFont);
}

void MenuScenario::HandleEvents(const Event& windowEvent)
{

}