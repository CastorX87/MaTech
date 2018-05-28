#pragma once
#include "stdafx.h"
#include "TextureManager.h"
#include "Utility.h"

using namespace sf;
using namespace std;

enum class ObjectFeatureSet
{
	OFS_NOTHING = 0x00000000,
	OFS_UNIQUE = 0x00000001,
	OFS_TRANSFORMABLE = 0x00000002,
	OFS_DRAWABLE = 0x00000004,
	OFS_BACKGROUND = 0x00000008 | OFS_DRAWABLE,
	OFS_PHYSICAL = 0x00000010 | OFS_TRANSFORMABLE,
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
		DebugPrintInfo(String("UniqueObject::UniqueObject()"));
		ExtendFeatureSet(ObjectFeatureSet::OFS_TRANSFORMABLE);
		mPos = pos;
		mAngle = angle;
	}
	virtual ~BaseTransformable()
	{
		DebugPrintInfo(String("UniqueObject::~UniqueObject()"));
	}

	virtual b2Vec2	GetPosition() const									{ return mPos; }
	virtual float		GetAngle() const										{ return mAngle; }
	virtual void		SetPosition(const b2Vec2& newPos)		{ mPos = newPos; };
	virtual void		SetAngle(float newAngle)						{ mAngle = newAngle; };
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
	}

	virtual void Draw(RenderTexture* renderTexture) const;
};

class BasePhysicalBody :
	public BaseTransformable
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
