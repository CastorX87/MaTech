#include "stdafx.h"
#include "MaTechApp.h"
#include "BaseButton.h"

using namespace sf;

MaTechApp::MaTechApp(String title, const Vector2i& viewportRes)
{
	VideoMode videoMode(viewportRes.x, viewportRes.y, 24);
	ContextSettings contextSettings(24, 0, 4, 3, 0, 0, false);
	Uint32 style = Style::Default;
	mMainWindow = new RenderWindow(videoMode, title, style, contextSettings);

	mRenderTexture = new RenderTexture();
	ContextSettings contextSettingsRenderTex;
	contextSettingsRenderTex.antialiasingLevel = 4;
	mRenderTexture->create(viewportRes.x, viewportRes.y, contextSettingsRenderTex);
	mRenderTexture->setSmooth(true);

	mMainSprite = new Sprite();
	mMainSprite->setTexture(mRenderTexture->getTexture());
}

MaTechApp::~MaTechApp()
{
	//SafeDelete(mScenarioPtr);
	SafeDelete(mMainSprite);
	SafeDelete(mRenderTexture);
	SafeDelete(mMainWindow);
}

void MaTechApp::Run()
{
	sf::Clock clock;

	while (mMainWindow->isOpen())
	{
		sf::Event event;
		while (mMainWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				mMainWindow->close();

			for (auto object : mScenarioPtr->GetScene()->GetObjects())
			{
				if (object.second->HasFeatureSet(ObjectFeatureSet::OFS_EVENT_HANDLER))
				{
					((BaseEventHandler*)dynamic_cast<BaseEventHandler*>(object.second))->HandleEvent(event);
				}
			}

		}

		sf::Time elapsed = clock.restart();
		Physics(elapsed);
		Render();
	}
}

void MaTechApp::Physics(const sf::Time& elapsed)
{
	mScenarioPtr->AdvanceTime(elapsed.asSeconds());

	//objPtr = mScenarioPtr->GetScene()->GetObject(L"btnQuit");
	//BaseButton* btnPtr = ((BaseButton*)dynamic_cast<BaseButton*>(objPtr));
	//btnPtr->SetPos(hpos);
	//btnPtr->SetAngle(angle);

}

void MaTechApp::Render()
{
	mRenderTexture->clear(Color(0, 0, 0, 0));
	mMainWindow->clear(Color(0, 0, 0, 0));
	
	sf::RenderStates rs;

	for (auto object : mScenarioPtr->GetScene()->GetObjects())
	{
		if (object.second->HasFeatureSet(ObjectFeatureSet::OFS_DRAWABLE))
		{
			((BaseDrawable*)dynamic_cast<BaseDrawable*>(object.second))->Draw(mRenderTexture, mScenarioPtr->GetCamera(), rs);
		}
	}

	mRenderTexture->display();
	mMainWindow->draw(*mMainSprite);
	mMainWindow->display();
}