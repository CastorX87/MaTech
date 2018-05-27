#pragma once
#include "stdafx.h"

using namespace sf;

enum class ObjectFeatureSet
{
	OFS_NOTHING = 0x00000000,
	OFS_UNIQUE = 0x00000001,
	OFS_TRANSFORMABLE = 0x00000002,
	OFS_DRAWABLE = 0x00000004,
	OFS_PHYSICAL = 0x00000008 | OFS_TRANSFORMABLE,
	OFS_SIMPLE_REAL_OBJECT = OFS_UNIQUE | OFS_PHYSICAL | OFS_DRAWABLE,
};

class BaseObject
{
protected:
	ObjectFeatureSet	mFeatureSet;

	BaseObject()
	{
		mFeatureSet = ObjectFeatureSet::OFS_NOTHING;
	}

	void ExtendFeatureSet(ObjectFeatureSet extension)
	{
		mFeatureSet = (ObjectFeatureSet)((int)mFeatureSet | (int)extension);
	}

	virtual ~BaseObject()
	{
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

};

class UniqueObject :
	virtual public BaseObject
{
protected:
	String mName;

public:
	UniqueObject(const String& name)
	{
		mName = name;
		ExtendFeatureSet(ObjectFeatureSet::OFS_UNIQUE);
	}

	virtual ~UniqueObject()
	{
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
		ExtendFeatureSet(ObjectFeatureSet::OFS_TRANSFORMABLE);
		mPos = pos;
		mAngle = angle;
	}
	virtual ~BaseTransformable()	{	}

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
		mDrawPriority = drawPriority;
		ExtendFeatureSet(ObjectFeatureSet::OFS_DRAWABLE);
	}
	virtual ~BaseDrawable() {  }

	virtual void Draw() = 0;
	virtual float GetDrawPriority() const								{ return mDrawPriority; }
	virtual void SetDrawPriority(float newDrawPriority) {	mDrawPriority = newDrawPriority; }
};

class BasePhysicalBody :
	public BaseTransformable
{
protected:
	b2World *mWorldPtr;

	b2Shape mShape;
	b2BodyDef mBodyDef;
	b2Body *mBody;

public:
	BasePhysicalBody() :
		BaseTransformable()
	{
		ExtendFeatureSet(ObjectFeatureSet::OFS_PHYSICAL);
	}

	BasePhysicalBody(b2World* world, b2Shape* shape, float density, float friction, const b2Vec2& pos = b2Vec2(0, 0), float angle = 0) :
		BaseTransformable(pos, angle)
	{
		ExtendFeatureSet(ObjectFeatureSet::OFS_PHYSICAL);
		//mBody = physBody;
		mBody->SetTransform(pos, angle);
	}

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

	b2Body* GetPhysBody() { return mBody; }
	const b2Body* GetPhysBody() const { return mBody; }

	void SetPhysBody(b2Body* newRigidBody) { mBody = newRigidBody; }

	virtual ~BasePhysicalBody()
	{
		mWorldPtr->DestroyBody(mBody);
	};
};
//
//class SimpleRealObject :
//	public UniqueObject,
//	public BasePhysicalBody,
//	public BaseDrawable
//{
//protected:
//	Model * mModel;
//
//public:
//	SimpleRealObject(const wstring& name, const wstring& modelName, const btTransform& transform);
//
//	const Model* GetModel() const
//	{
//		return mModel;
//	}
//
//	Model* GetModel()
//	{
//		return mModel;
//	}
//
//	virtual void Draw(Graphics::RenderMode renderMode, const glm::mat4& matV, const glm::mat4& matP);
//
//	virtual bool IsVisibleInFrustum(const Frustum& frustum) const
//	{
//		btVector3 aabbMin, aabbMax;
//		GetRigidBody()->getAabb(aabbMin, aabbMax);
//		return frustum.IntersectAabb(Util::BtVec3ToVec3(aabbMin), Util::BtVec3ToVec3(aabbMax));
//	}
//
//	virtual ~SimpleRealObject()
//	{
//		ResourceManager::ReleaseModel(mModel);
//	}
//};
//
//class Planet :
//	public SimpleRealObject
//{
//protected:
//	Material * mPlanetMaterial;
//	Material* mPlanetRingModel;
//
//public:
//	Planet(const wstring& name, const wstring& planetMaterialFileName, const wstring& ringMaterialFileName, const btTransform& transform = btTransform::getIdentity()) :
//		SimpleRealObject(name, L"PLANET_BASE", transform)
//	{
//		ExtendFeatureSet(ObjectFeatureSet::OFS_PLANET);
//		mPlanetMaterial = ResourceManager::LoadMaterialFromFile(planetMaterialFileName);
//		mPlanetRingModel = ResourceManager::LoadMaterialFromFile(ringMaterialFileName);
//		std::cout << (int)this->GetFeatureSet() << endl;
//	}
//
//	virtual void Draw(Graphics::RenderMode renderMode, const glm::mat4& matV, const glm::mat4& matP);
//
//	virtual ~Planet()
//	{
//		ResourceManager::ReleaseMaterial(mPlanetMaterial);
//		ResourceManager::ReleaseMaterial(mPlanetRingModel);
//	}
};