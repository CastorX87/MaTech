#include "stdafx.h"
#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	for_each(mObjects.begin(), mObjects.end(), [](auto obj) { SafeDelete(obj); });
}

BaseObject* Scene::AddObject(BaseObject* newObject)
{
	mObjects.push_back(newObject);
	return newObject;
}

void Scene::DeleteObject(BaseObject* object)
{
	std::remove(mObjects.begin(), mObjects.end(), object);
	delete object;
}
