#pragma once
#include "stdafx.h"
#include "SceneObject.h"

using namespace std;
using namespace sf;

class Scene
{
private:
	vector<BaseObject*> mObjects;

public:
	Scene();
	~Scene();

	BaseObject* AddObject(BaseObject* newObject);
	void DeleteObject(BaseObject* object);

	const vector<BaseObject*>& GetObjects() const { return mObjects; }
};