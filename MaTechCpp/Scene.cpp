#include "stdafx.h"
#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	for (auto& obj: mObjects)
	{
		delete obj.second;
	}
	mObjects.clear();
}

BaseObject* Scene::AddObject(const String& name, BaseObject* newObject)
{
	mObjects.insert(std::pair<String, BaseObject*>(name, newObject));
	return newObject;
}

void Scene::DeleteObject(BaseObject* object)
{
	String name;
	for (auto& obj : mObjects)
	{
		if (obj.second == object)
		{
			name = obj.first;
			delete obj.second;
		}
	}
	mObjects.erase(name);
}

void Scene::DeleteObject(const String& name)
{
	BaseObject* obj = mObjects[name];
	mObjects.erase(name);
	delete obj;
}
