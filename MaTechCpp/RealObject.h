#pragma once
#include "stdafx.h"
#include "SceneObject.h"
#include "Camera.h"

class RealObject :
	public BaseDrawable,
	public BasePhysicalBody
{
protected:
	Texture * mTexturePtr;
	const Camera* mCameraPtr;
	Sprite* mSprite;
	Text* mText;
	Vector2f mSize;

public:
	RealObject(const String& textureName, const String& text, const Font& font, int charSize, int layerId, int drawPriority, const Vector2f& size, const Camera* camPtr, b2World* world, const b2BodyDef& bodyDef, const vector<b2FixtureDef>& fixtureDefs) :
		BaseDrawable(layerId, drawPriority),
		BasePhysicalBody(world, bodyDef, fixtureDefs)
	{
		mTexturePtr = TextureManager::GetInstance().LoadTexture(textureName);
		mSprite = new Sprite();
		mSprite->setTexture(*mTexturePtr);
		mText = nullptr;
		mSize = size;
		mCameraPtr = camPtr;
		if (text.isEmpty() == false)
		{
			SetText(text, font, charSize);
		}
	}

	virtual ~RealObject()
	{
		DebugPrintInfo(String("RealObject::~RealObject()"));
		SafeDelete(mText);
		SafeDelete(mSprite);
		TextureManager::GetInstance().ReleaseTexture(mTexturePtr);
	}

	void SetText(const String& text, const Font& font, int charSize)
	{
		SafeDelete(mText);
		if (text.isEmpty() == false)
		{
			mText = new Text(text, font, charSize);
			mText->setFillColor(Color::Red);
		}
	}

	virtual void Draw(RenderTexture* renderTexture, const Camera& cam, const RenderStates& rs) const
	{
		FloatRect spriteBounds = mSprite->getLocalBounds();
		RenderStates rsLocal = rs;
		Transform T = BaseDrawable::GetScreenTransformRasterObj(this->GetHPosition(), mSize, this->GetAngle(), mSize * 0.5f, Vector2i(spriteBounds.width, spriteBounds.height), cam);
		rsLocal.transform = T;
		renderTexture->draw(*mSprite, rsLocal);

		if (mText != nullptr)
		{
			FloatRect textBounds = mText->getLocalBounds();
			RenderStates rsLocal = rs;
			Transform T = BaseDrawable::GetScreenTransformRasterObj(this->GetHPosition(), mSize, this->GetAngle(), mSize * 0.5f, Vector2i(textBounds.width, textBounds.height), cam);
			rsLocal.transform = T;
			renderTexture->draw(*mText, rsLocal);
		}
	}
};