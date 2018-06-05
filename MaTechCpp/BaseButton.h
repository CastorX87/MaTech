#pragma once
#include "stdafx.h"
#include "SceneObject.h"

class BaseButton :
	public UniqueObject,
	public BaseDrawable,
	public BaseTransformable,
	public BaseScaleable,
	public BaseEventHandler
{
protected:
	Texture * mTexturePtr;
	Sprite* mSprite;

	static int ButtonEventHandler(const Event& event, void* customPtr)
	{
		printf("BaseButton::ButtonEventHandler\n");
		return 0;
	}

public:
	BaseButton(const String& name, const String& textureName, float drawPriority, const b2Vec2& centerPos, float angle, const b2Vec2& scale) :
		UniqueObject(name),
		BaseDrawable(drawPriority),
		BaseTransformable(centerPos, angle),
		BaseScaleable(scale),
		BaseEventHandler(ButtonEventHandler, (void*)this)
	{
		mTexturePtr = TextureManager::GetInstance().LoadTexture(textureName);
		mSprite = new Sprite();
		mSprite->setTexture(*mTexturePtr);
	}

	virtual ~BaseButton()
	{
		DebugPrintInfo(String("BaseButton::~BaseButton()"));
		SafeDelete(mSprite);
		TextureManager::GetInstance().ReleaseTexture(mTexturePtr);
	}

	virtual void Draw(RenderTexture* renderTexture) const
	{
		renderTexture->draw(*mSprite);
	}
};