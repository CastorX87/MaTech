#pragma once
#ifndef _TM_H_
#define _TM_H_

#include "stdafx.h"
#include "NonCopyable.h"

using namespace sf;
using namespace std;

struct TextManMapVal
{
	Texture* texPtr;
	int refCount;
};

class TextureManager :
	public NonCopyableClass
{
public:
	map<string, TextManMapVal> mTextures;

public:
	inline TextureManager() { }
	inline ~TextureManager() { }
	static TextureManager& GetInstance()
	{
		static TextureManager instance;
		return instance;
	}

	Texture* LoadTexture(String path)
	{
		if (mTextures.find(path) == mTextures.end())
		{
			mTextures[path].texPtr = new Texture();
			mTextures[path].texPtr->setSmooth(true); // Enable smoothing
			mTextures[path].refCount = 0;
			if (mTextures[path].texPtr->loadFromFile(path) == false)
				throw exception("Texture loading error");
		}
		mTextures[path].refCount++;
		return mTextures[path].texPtr;
	}

	void ReleaseTexture(Texture* texPtr)
	{
		// Search for the texture to be released
		auto it = mTextures.begin();
		while (it != mTextures.end())
		{
			if(it->second.texPtr == texPtr)
				it->second.refCount--;
			it++;
		}

		// Check if there are any textures that has to be freed (refCount <= 0)
		it = mTextures.begin();
		while (it != mTextures.end())
		{
			if (it->second.refCount <= 0)
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

#endif