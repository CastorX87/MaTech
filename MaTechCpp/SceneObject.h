#pragma once
#include "stdafx.h"
#include "TextureManager.h"
#include "Utility.h"
#include "Camera.h"

using namespace sf;
using namespace std;

enum class ObjectFeatureSet
{
	OFS_NOTHING					= 1 << 0,		// Base object ha no special features
	OFS_DRAWABLE				= 1 << 2,		// Stuff to draw
	OFS_EVENT_HANDLER			= 1 << 3,		// Stuff to notify when new event is received
	OFS_PHYSICAL				= 1 << 4,		// Must be updated by Box2D
};

class BaseObject :
	public NonCopyable
{
protected:
	ObjectFeatureSet	mFeatureSet;

protected:
	void ExtendFeatureSet(ObjectFeatureSet extension)
	{
		mFeatureSet = (ObjectFeatureSet)((int)mFeatureSet | (int)extension);
	}

	BaseObject()
	{
		DebugPrintInfo("BaseObject::BaseObject()");
		mFeatureSet = ObjectFeatureSet::OFS_NOTHING;
	}

public:

	ObjectFeatureSet GetFeatureSet() const
	{
		return mFeatureSet;
	}

	bool HasFeatureSet(ObjectFeatureSet featureSet) const
	{
		return ((int)mFeatureSet & (int)featureSet) == (int)featureSet;
	}

	virtual ~BaseObject()
	{
		DebugPrintInfo("BaseObject::~BaseObject()");
	}
};

class BaseDrawable :
	virtual public BaseObject
{
protected:
	int mLayerId;
	int mDrawPriority;

public:
	static Transform GetScreenTransformRasterObj(Vector2f translation, Vector2f scale, float rotation, Vector2f hCenterOffset, Vector2i extendInPixels, Camera cam)
	{
		float asp = cam.ViewportSize.x / (float)cam.ViewportSize.y;
		Transform TCamPos = Transform().translate(cam.CenterPos.x, -cam.CenterPos.y);
		Transform TRot = Transform().rotate(rotation, Vector2f(hCenterOffset.x, scale.y - hCenterOffset.y));
		Transform TCenterOffset = Transform().translate(Vector2f(hCenterOffset.x, scale.y - hCenterOffset.y));
		Transform TCameraOffset = Transform().translate(Vector2f(0.5, (1 / asp) / 2));
		Transform TPos = Transform().translate(Vector2f(translation.x, -translation.y));
		Transform TScale = Transform().scale(scale);
		Transform TRealToHSize = Transform().scale(Vector2f((float)extendInPixels.x / cam.ViewportSize.x, (float)extendInPixels.y / cam.ViewportSize.x));
		Transform TScreen = Transform().scale(cam.ViewportSize.x, cam.ViewportSize.x);
		Transform TZoom = Transform().scale(cam.ZoomFactor, cam.ZoomFactor, cam.ViewportSize.x / 2, cam.ViewportSize.y / 2);

		Transform T = TZoom * TScreen * TCenterOffset.getInverse() * TCamPos.getInverse() * TCameraOffset * TPos  * TRot * TRealToHSize.getInverse() * TScale * TScreen.getInverse();

		return T;
	}

	static Transform GetScreenTransform(Vector2f translation, Vector2f scale, float rotation, Vector2f hCenterOffset, Camera cam)
	{
		return GetScreenTransformRasterObj(translation, scale, rotation, hCenterOffset, Vector2i(cam.ViewportSize.x, cam.ViewportSize.x), cam);
	}

	static Transform GetGlobalToLocalTransform(Vector2f translation, float rotation)
	{
		Transform T;
		Transform TRot = Transform().rotate(-rotation);
		Transform TPos = Transform().translate(Vector2f(translation.x, translation.y));
		T = (TPos * TRot).getInverse();
		return T;
	}

public:
	BaseDrawable(int layerId, int drawPriority) :
		BaseObject()
	{
		DebugPrintInfo(String("BaseDrawable::BaseDrawable()"));
		mLayerId = layerId;
		mDrawPriority = drawPriority;
		ExtendFeatureSet(ObjectFeatureSet::OFS_DRAWABLE);
	}

	virtual ~BaseDrawable()
	{
		DebugPrintInfo(String("BaseDrawable::~BaseDrawable()"));
	}

	virtual void Draw(RenderTexture* renderTexture, const Camera& cam, const RenderStates& rs) const = 0;
	virtual int GetDrawPriority() const									{ return mDrawPriority; }
	virtual void SetDrawPriority(int newDrawPriority)		{	mDrawPriority = newDrawPriority; }
	virtual int GetLayerId() const											{ return mLayerId; }
	virtual void SetLayerId(int layerId)								{ mLayerId = layerId; }
};

class BaseEventHandler :
	virtual public BaseObject
{
protected:
	function<int(const Event&, void*)> mFunction;
	void* mCustomObjPtr;

	BaseEventHandler(const BaseEventHandler&) = delete;

public:
	BaseEventHandler(const function<int(const Event&, void*)>& eventFunction, void* customObjPtr) :
		BaseObject()
	{
		DebugPrintInfo(String("EventReceiver::EventReceiver()"));
		ExtendFeatureSet(ObjectFeatureSet::OFS_EVENT_HANDLER);
		mCustomObjPtr = customObjPtr;
		mFunction = eventFunction;
	}

	virtual ~BaseEventHandler()
	{
		DebugPrintInfo(String("EventReceiver::~EventReceiver()"));
	}

	virtual int HandleEvent(const Event& event)
	{
		return mFunction(event, mCustomObjPtr);
	};
};


class BasePhysicalBody :
	virtual public BaseObject
{
protected:
	b2World * mWorldPtr;									// Must NOT be deleted!
	b2Body *mBody;												// Rigid body object. Must be deleted using mWorldPtr
	vector<b2Fixture*> mBodyFixturePtrs;	// List of fixtures used by this object. All of them must be deleted using mBody

public:
	BasePhysicalBody(const String& name) :
		BaseObject()
	{
		DebugPrintInfo(String("BasePhysicalBody::BasePhysicalBody()"));
		ExtendFeatureSet(ObjectFeatureSet::OFS_PHYSICAL);
	}

	BasePhysicalBody(b2World* world, const b2BodyDef& bodyDef, const vector<b2FixtureDef>& fixtureDefs)
	{
		mWorldPtr = world;

		ExtendFeatureSet(ObjectFeatureSet::OFS_PHYSICAL);
		mBody = mWorldPtr->CreateBody(&bodyDef);
		for(auto& fdef : fixtureDefs)
		{
			mBodyFixturePtrs.push_back(mBody->CreateFixture(&fdef));
		}
		SetHPosition(Vector2f(bodyDef.position.x, bodyDef.position.y));
		SetAngle(bodyDef.angle);
	}
	virtual ~BasePhysicalBody()
	{
		DebugPrintInfo(String("BasePhysicalBody::~BasePhysicalBody()"));
		for (auto fixturePtr : mBodyFixturePtrs)
		{
			mBody->DestroyFixture(fixturePtr);
		}
		mWorldPtr->DestroyBody(mBody);
	};

	virtual size_t GetFixtureCount() { return mBodyFixturePtrs.size(); }
	virtual b2Fixture* GetFixturePtr(int i) { return mBodyFixturePtrs[i]; }
	virtual const b2Fixture* GetFixturePtr(int i) const { return mBodyFixturePtrs[i]; }
	virtual Vector2f GetHPosition() const
	{
		return Vector2f(mBody->GetPosition().x, mBody->GetPosition().y);
	}
	virtual float GetAngle() const
	{
		return mBody->GetAngle() * 180 / -3.14;
	}
	virtual void SetHPosition(const Vector2f& newPos)
	{
		mBody->SetTransform(b2Vec2(newPos.x, newPos.y), GetAngle());
	}
	virtual void SetAngle(float newAngle)
	{
		mBody->SetTransform(b2Vec2(GetHPosition().x, GetHPosition().y), newAngle);
	}

	b2Body* GetBody() { return mBody; }
	const b2Body* GetBody() const { return mBody; }
};
/*
class SimpleRealObject :
	public UniqueObject,
	public BasePhysicalBody,
	public BaseDrawable
{
protected:
	Texture* mTexturePtr;
	Sprite* mSprite;

public:
	SimpleRealObject(const String& name, const String& textureName, b2World* world, const b2BodyDef& bodyDef, const vector<b2FixtureDef>& fixtureDefs, int layerId, int drawPriority, const Vector2f& hSize) :
		UniqueObject(name),
		BaseDrawable(layerId, drawPriority, hSize),
		BasePhysicalBody(world, bodyDef, fixtureDefs)
	{
		DebugPrintInfo(String("SimpleRealObject::SimpleRealObject()"));
		mTexturePtr = TextureManager::GetInstance().LoadTexture(textureName);
		mSprite = new Sprite();
		mSprite->setTexture(*mTexturePtr);
	}

	const Texture* GetTexturePtr() const
	{
		return mTexturePtr;
	}

	Texture* GetTexturePtr()
	{
		return mTexturePtr;
	}

	virtual void Draw(RenderTexture* renderTexture, const Camera& cam) const;

	virtual ~SimpleRealObject()
	{
		DebugPrintInfo(String("SimpleRealObject::~SimpleRealObject()"));
		TextureManager::GetInstance().ReleaseTexture(mTexturePtr);
		SafeDelete(mSprite);
	}

};*/
