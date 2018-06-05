#pragma once
#include "stdafx.h"
#include "TextureManager.h"
#include "Utility.h"

using namespace sf;
using namespace std;

enum class ObjectFeatureSet
{
	OFS_NOTHING = 1 << 0,
	OFS_UNIQUE = 1 << 1,
	OFS_TRANSFORMABLE = 1 << 2,
	OFS_DRAWABLE = 1 << 3,
	OFS_SCALEABLE = 1 << 4,
	OFS_EVENT_HANDLER = 1 << 5,
	OFS_BACKGROUND = 1 << 6 | OFS_DRAWABLE,
	OFS_PHYSICAL = 1 << 7 | OFS_TRANSFORMABLE,
	OFS_CONTROL = 1 << 8 | OFS_DRAWABLE | OFS_TRANSFORMABLE | OFS_SCALEABLE,
	OFS_SIMPLE_REAL_OBJECT = OFS_UNIQUE | OFS_PHYSICAL | OFS_DRAWABLE,
};

class BaseObject :
	public NonCopyable
{
protected:
	ObjectFeatureSet	mFeatureSet;

	BaseObject()
	{
		DebugPrintInfo("BaseObject::BaseObject()");
		mFeatureSet = ObjectFeatureSet::OFS_NOTHING;
	}

	void ExtendFeatureSet(ObjectFeatureSet extension)
	{
		mFeatureSet = (ObjectFeatureSet)((int)mFeatureSet | (int)extension);
	}

public:
	ObjectFeatureSet GetFeatureSet() const
	{
		return mFeatureSet;
	}

	bool HasFeatureSet(ObjectFeatureSet featureSet)
	{
		return ((int)mFeatureSet & (int)featureSet) == (int)featureSet;
	}

	virtual ~BaseObject()
	{
		DebugPrintInfo("BaseObject::~BaseObject()");
	}
};

class UniqueObject :
	virtual public BaseObject
{
protected:
	String mName;

public:
	UniqueObject(const String& name)
	{
		DebugPrintInfo(String("UniqueObject::UniqueObject(") + name + ")");
		mName = name;
		ExtendFeatureSet(ObjectFeatureSet::OFS_UNIQUE);
	}

	virtual ~UniqueObject()
	{
		DebugPrintInfo(String("UniqueObject::~UniqueObject(") + mName + ")");
	}

	const String& GetName() const
	{
		return mName;
	}
};

class BaseTransformable :
	virtual public BaseObject
{
protected:
	b2Vec2 mPos;
	float mAngle;

public:
	BaseTransformable(const b2Vec2& pos = b2Vec2(0, 0), float angle = 0)
	{
		DebugPrintInfo(String("BaseTransformable::BaseTransformable()"));
		ExtendFeatureSet(ObjectFeatureSet::OFS_TRANSFORMABLE);
		mPos = pos;
		mAngle = angle;
	}
	virtual ~BaseTransformable()
	{
		DebugPrintInfo(String("BaseTransformable::~BaseTransformable()"));
	}

	virtual b2Vec2	GetPosition() const									{ return mPos; }
	virtual float		GetAngle() const										{ return mAngle; }
	virtual void		SetPosition(const b2Vec2& newPos)		{ mPos = newPos; };
	virtual void		SetAngle(float newAngle)						{ mAngle = newAngle; };
};

class BaseScaleable :
	virtual public BaseObject
{
protected:
	b2Vec2 mScale;

public:
	BaseScaleable(const b2Vec2& scale = b2Vec2(0, 0))
	{
		DebugPrintInfo(String("BaseScaleable::BaseScaleable()"));
		ExtendFeatureSet(ObjectFeatureSet::OFS_TRANSFORMABLE);
		mScale = scale;
	}
	virtual ~BaseScaleable()
	{
		DebugPrintInfo(String("BaseScaleable::~BaseScaleable()"));
	}

	virtual b2Vec2	GetScale() const { return mScale; }
	virtual void		SetScale(const b2Vec2& newScale) { mScale = newScale; };
};

class BaseDrawable :
	virtual public BaseObject
{
protected:
	float mDrawPriority; // greater number -> smaller number == draw first -> draw last

public:
	BaseDrawable(float drawPriority = FLT_MAX)
	{
		DebugPrintInfo(String("BaseDrawable::BaseDrawable()"));
		mDrawPriority = drawPriority;
		ExtendFeatureSet(ObjectFeatureSet::OFS_DRAWABLE);
	}
	virtual ~BaseDrawable()
	{
		DebugPrintInfo(String("BaseDrawable::~BaseDrawable()"));
	}

	virtual void Draw(RenderTexture* renderTexture) const = 0;
	virtual float GetDrawPriority() const								{ return mDrawPriority; }
	virtual void SetDrawPriority(float newDrawPriority) {	mDrawPriority = newDrawPriority; }
};

class Background :
	virtual public BaseDrawable
{
protected:
	Texture * mTexturePtr;
	Sprite* mSprite;
public:
	Background(const String& textureName) :
		BaseDrawable(0)
	{
		DebugPrintInfo(String("BackgroundDrawable::BackgroundDrawable()"));
		mTexturePtr = TextureManager::GetInstance().LoadTexture(textureName);
		mSprite = new Sprite();
		mSprite->setTexture(*mTexturePtr);
		ExtendFeatureSet(ObjectFeatureSet::OFS_BACKGROUND);
	}
	virtual ~Background()
	{
		DebugPrintInfo(String("BackgroundDrawable::~BackgroundDrawable()"));
		SafeDelete(mSprite);
		TextureManager::GetInstance().ReleaseTexture(mTexturePtr);
	}

	virtual void Draw(RenderTexture* renderTexture) const;
};

class BaseEventHandler :
	virtual public BaseObject
{
protected:
	function<int(const Event&, void*)> mFunction;
	void* mCustomObjPtr;

public:
	BaseEventHandler(function<int(const Event&, void*)> eventFunction, void* customObjPtr)
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
	virtual public BaseTransformable
{
protected:
	b2World * mWorldPtr;									// Must NOT be deleted!
	b2Body *mBody;												// Rigid body object. Must be deleted using mWorldPtr
	vector<b2Fixture*> mBodyFixturePtrs;	// List of fixtures used by this object. All of them must be deleted using mBody

public:
	BasePhysicalBody() :
		BaseTransformable()
	{
		DebugPrintInfo(String("BasePhysicalBody::BasePhysicalBody()"));
		ExtendFeatureSet(ObjectFeatureSet::OFS_PHYSICAL);
	}

	BasePhysicalBody(b2World* world, const b2BodyDef& bodyDef, const vector<b2FixtureDef>& fixtureDefs) :
		BaseTransformable(bodyDef.position, bodyDef.angle)
	{
		ExtendFeatureSet(ObjectFeatureSet::OFS_PHYSICAL);
		mWorldPtr->CreateBody(&bodyDef);
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
	virtual b2Vec2 GetPosition() const override
	{
		return mBody->GetPosition();
	}
	virtual float GetAngle() const override
	{
		return mBody->GetAngle();
	}
	virtual void SetPosition(const b2Vec2& newPos) override
	{
		mBody->SetTransform(newPos, GetAngle());
	}
	virtual void SetAngle(float newAngle) override
	{
		mBody->SetTransform(GetPosition(), newAngle);
	}
	b2Body* GetBody() { return mBody; }
	const b2Body* GetBody() const { return mBody; }
};

class SimpleRealObject :
	public UniqueObject,
	public BasePhysicalBody,
	public BaseDrawable
{
protected:
	Texture* mTexturePtr;
	Sprite* mSprite;

public:
	SimpleRealObject(const String& name, const String& textureName, b2World* world, const b2BodyDef& bodyDef, const vector<b2FixtureDef>& fixtureDefs) :
		UniqueObject(name),
		BaseDrawable(),
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

	virtual void Draw(RenderTexture* renderTexture) const;

	virtual ~SimpleRealObject()
	{
		DebugPrintInfo(String("SimpleRealObject::~SimpleRealObject()"));
		TextureManager::GetInstance().ReleaseTexture(mTexturePtr);
		SafeDelete(mSprite);
	}

};
