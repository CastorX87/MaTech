#pragma once
#include "BaseScenario.h"

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
}


MenuScenario::~MenuScenario()
{
}

void MenuScenario::HandleEvents(const Event& windowEvent)
{

}