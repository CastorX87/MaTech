#pragma once
#include "stdafx.h"
#include "NonCopyable.h"

using namespace sf;
using namespace std;

class TextureManager :
	public NonCopyableClass
{
private:
	map<string, shared_ptr<Texture>> mTextures;

public:
	inline TextureManager() { }
	inline ~TextureManager() { }

	shared_ptr<Texture> LoadTexture(String path)
	{
		if (mTextures.find(path) == mTextures.end())
		{
			mTextures[path] = shared_ptr<Texture>(new Texture());
			if (mTextures[path]->loadFromFile(path) == false)
				throw exception("Texture loading error");
		}
		return mTextures[path];
	}

	void ReleaseUnused()
	{
		auto it = mTextures.begin();
		while (it != mTextures.end())
		{
			if (it->second.use_count() == 1)
			{
				it = mTextures.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
};