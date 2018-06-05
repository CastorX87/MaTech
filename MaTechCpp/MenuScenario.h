#pragma once
#include "BaseScenario.h"
#include "BaseButton.h"

class MenuScenario :
	public BaseScenario
{
public:
	MenuScenario();
	virtual ~MenuScenario();
	virtual void HandleEvents(const Event& windowEvent);
};


MenuScenario::MenuScenario() :
	BaseScenario()
{
	mScene->AddObject(new Background("Resources\\Textures\\pngtree_test.png"));
	mScene->AddObject(new BaseEventHandler([](const Event& e, void* ptr) -> bool { return printf("Event Received\n"); }, nullptr));
	mScene->AddObject(new BaseButton("BtnExit", "Resources\\Textures\\ButtonBackgrountTest.png", 0, b2Vec2(0, 0), 0, b2Vec2(1, 1)));
}


MenuScenario::~MenuScenario()
{
}

void MenuScenario::HandleEvents(const Event& windowEvent)
{

}