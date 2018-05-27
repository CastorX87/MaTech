#include "stdafx.h"
#include "MaTechApp.h"

using namespace sf;

MaTechApp::MaTechApp(String title, int width, int height)
{
	VideoMode videoMode(width, height, 24);
	ContextSettings contextSettings(32, 0, 4, 3, 0, 0, false);
	Uint32 style = Style::Default;
	mMainWindow = new RenderWindow(videoMode, title, style, contextSettings);

	mScene = new Scene();
	mRenderTexture = new RenderTexture();
	ContextSettings contextSettingsRenderTex;
	contextSettingsRenderTex.antialiasingLevel = 4;
	mRenderTexture->create(width, height, contextSettingsRenderTex);
	mRenderTexture->setSmooth(true);

	mMainSprite = new Sprite();
	mMainSprite->setTexture(mRenderTexture->getTexture());
}

MaTechApp::~MaTechApp()
{
	SafeDelete(mMainSprite);
	SafeDelete(mRenderTexture);
	SafeDelete(mScene);
	SafeDelete(mMainWindow);
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
		Render();
	}
}

void MaTechApp::Render()
{
	mRenderTexture->clear(Color(255,2,2,0));
	mMainWindow->clear(Color(255, 255, 0, 255));
	
	for (auto object : mScene->GetObjects())
	{
		if (object->HasFeatureSet(ObjectFeatureSet::OFS_DRAWABLE))
		{
			((BaseDrawable*)dynamic_cast<BaseDrawable*>(object))->Draw(mRenderTexture);
		}
	}
	mRenderTexture->display();
	mMainWindow->draw(*mMainSprite);
	mMainWindow->display();
}