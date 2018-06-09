#pragma once
#include "stdafx.h"
#include "SceneObject.h"

class BaseButton :
	public BaseDrawable,
	public BaseEventHandler
{
public:
	struct EventFucAndUserPtr
	{
		function<int(const Event&, void*)> Function;
		void* UserPtr;

		EventFucAndUserPtr(function<int(const Event&, void*)> function, void* userPtr)
		{
			Function = function;
			UserPtr = userPtr;
		}
	};

protected:
	Texture* mTexturePtr;
	Sprite* mSprite;
	Text* mText;
	EventFucAndUserPtr* mOnClickData;
	Vector2f mPos;
	Vector2f mSize;

	static int ButtonEventHandler(const Event& event, void* customPtr)
	{
		if (customPtr != nullptr)
		{
			auto thisButton = ((BaseButton*)customPtr);
			if (((BaseButton*)customPtr)->mOnClickData->Function)
			{
				((BaseButton*)customPtr)->mOnClickData->Function(event, ((BaseButton*)customPtr)->mOnClickData->UserPtr);
			}
		}
		return 0;
	}

public:
	BaseButton(const String& textureName, const String& text, const Font& font, int charSize, int layerId, int drawPriority, const Vector2f& pos, const Vector2f& size) :
		BaseDrawable(layerId, drawPriority),
		BaseEventHandler(ButtonEventHandler, (void*)this),
		BaseObject()
	{
		mTexturePtr = TextureManager::GetInstance().LoadTexture(textureName);
		mSprite = new Sprite();
		mSprite->setTexture(*mTexturePtr);
		mText = nullptr;
		mPos = pos;
		mSize = size;
		if (text.isEmpty() == false)
		{
			SetText(text, font, charSize);
		}
		mOnClickData = nullptr;
	}

	virtual ~BaseButton()
	{
		DebugPrintInfo(String("BaseButton::~BaseButton()"));
		SafeDelete(mText);
		SafeDelete(mSprite);
		TextureManager::GetInstance().ReleaseTexture(mTexturePtr);
		SafeDelete(mOnClickData);
	}

	void SetText(const String& text, const Font& font, int charSize)
	{
		SafeDelete(mText);
		if (text.isEmpty() == false)
		{
			mText = new Text(text, font, charSize);
			mText->setFillColor(Color::Black);
		}
	}

	void RegisterOnClick(const function<int(const Event&, void*)>& onClick, void* userPtr)
	{
		mOnClickData = new EventFucAndUserPtr(onClick, userPtr);
	}

	virtual void Draw(RenderTexture* renderTexture, const Camera& cam, const RenderStates& rs) const
	{
		FloatRect spriteBounds = mSprite->getLocalBounds();
		RenderStates rsLocal = rs;
		Transform T = BaseDrawable::GetScreenTransform(mPos, mSize, 45, mSize * 0.5f, Vector2i(spriteBounds.width, spriteBounds.height), cam);
		rsLocal.transform = T;
		renderTexture->draw(*mSprite, rsLocal);

		if (mText != nullptr)
		{
			renderTexture->draw(*mText);
		}
	}
};