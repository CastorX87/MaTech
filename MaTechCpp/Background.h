#pragma once
#pragma once
#include "stdafx.h"
#include "TextureManager.h"
#include "Utility.h"
#include "Camera.h"

class Background :
	virtual public BaseDrawable
{
protected:
	Texture * mTexturePtr;
	Sprite* mSprite;

public:
	Background(const String& textureName) :
		BaseDrawable(0, 0)
	{
		DebugPrintInfo(String("BackgroundDrawable::BackgroundDrawable()"));
		mTexturePtr = TextureManager::GetInstance().LoadTexture(textureName);
		mSprite = new Sprite();
		mSprite->setTexture(*mTexturePtr);
	}

	virtual ~Background()
	{
		DebugPrintInfo(String("BackgroundDrawable::~BackgroundDrawable()"));
		SafeDelete(mSprite);
		TextureManager::GetInstance().ReleaseTexture(mTexturePtr);
	}

	virtual void Draw(RenderTexture* renderTexture, const Camera& cam, const RenderStates& rs) const
	{
		//DebugPrintInfo("void Background::Draw(RenderTexture * renderTexture) const");
		auto targetSize = renderTexture->getSize();
		renderTexture->draw(*mSprite);
	}

};
