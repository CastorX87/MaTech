#pragma once
#include "stdafx.h"
#include "SceneObject.h"
#include "Camera.h"

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
	Texture * mTexturePtr;
	const Camera * mCameraPtr;
	Sprite* mSprite;
	Text* mText;
	EventFucAndUserPtr* mOnClickData;
	Vector2f mPos;
	Vector2f mSize;
	float mAngle;

	static int ButtonEventHandler(const Event& event, void* customPtr)
	{
		if (customPtr == nullptr)
			return 0;
		
		auto thisButton = ((BaseButton*)customPtr);

		if (event.type == Event::EventType::MouseButtonReleased)
		{
			Vector2f TP = ScreenPosToHPos(Vector2i(event.mouseButton.x, event.mouseButton.y), thisButton->mCameraPtr->ViewportSize, thisButton->mCameraPtr->ZoomFactor);
			
			if (thisButton->IsPointInsideButton(TP) && ((BaseButton*)customPtr)->mOnClickData->Function)
			{
				((BaseButton*)customPtr)->mOnClickData->Function(event, ((BaseButton*)customPtr)->mOnClickData->UserPtr);
			}
		}


		return 0;
	}

public:
	BaseButton(const String& textureName, const String& text, const Font& font, int charSize, int layerId, int drawPriority, const Vector2f& pos, const Vector2f& size, float angle, const Camera* camPtr) :
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
		mAngle = angle;
		mCameraPtr = camPtr;
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

	const Sprite* GetSprite() const
	{
		return mSprite;
	}

	const Vector2f GetPos() const
	{
		return mPos;
	}

	void SetPos(const Vector2f& newPos)
	{
		mPos = newPos;
	}

	const Vector2f GetCenterOffset() const
	{
		return GetSize() * 0.5f;
	}

	const float GetAngle() const
	{
		return mAngle;
	}

	void SetAngle(float angle)
	{
		mAngle = angle;
	}

	const Vector2f GetSize() const
	{
		return mSize;
	}

	bool IsPointInsideButton(const Vector2f& pt)
	{
		Transform T_G2L = BaseDrawable::GetGlobalToLocalTransform(GetPos(), GetAngle());
		Vector2f transPt = T_G2L.transformPoint(pt);
		if (
			transPt.x > (-mSize.x / 2.0f) &&
			transPt.y > (-mSize.y / 2.0f) &&
			transPt.x < (mSize.x / 2.0f) &&
			transPt.y < (mSize.y / 2.0f)
			)
		{
			return true;
		}
		return false;
	}

	virtual void Draw(RenderTexture* renderTexture, const Camera& cam, const RenderStates& rs) const
	{
		FloatRect spriteBounds = mSprite->getLocalBounds();
		RenderStates rsLocal = rs;
		Transform T = BaseDrawable::GetScreenTransformRasterObj(mPos, mSize, mAngle, mSize * 0.5f, Vector2i(spriteBounds.width, spriteBounds.height), cam);
		rsLocal.transform = T;
		renderTexture->draw(*mSprite, rsLocal);

		if (mText != nullptr)
		{
			renderTexture->draw(*mText);
		}
	}
};