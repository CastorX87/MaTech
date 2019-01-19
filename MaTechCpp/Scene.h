#pragma once
#include "stdafx.h"
#include "SceneObject.h"

using namespace std;
using namespace sf;

class Scene
{
private:
	map<String, BaseObject*> mObjects;

public:
	Scene();
	~Scene();

	BaseObject* AddObject(const String& name, BaseObject* newObject);
	void DeleteObject(BaseObject* object);
	void DeleteObject(const String& name);

	const map<String, BaseObject*>& GetObjects() const { return mObjects; }

	BaseObject* GetObject(const String& name)
	{
		BaseObject* retPtr = nullptr;
		auto entry = mObjects.find(name);
		if (entry == mObjects.end())
			return retPtr;

		return (*entry).second;
	}

	const BaseObject* GetObject(const String& name) const
	{
		const BaseObject* retPtr = nullptr;
		auto entry = mObjects.find(name);
		if (entry == mObjects.end())
			return retPtr;

		return (*entry).second;
	}
};