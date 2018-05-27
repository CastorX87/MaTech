#include "stdafx.h"
#include "SceneObject.h"

void SimpleRealObject::Draw(RenderTexture* renderTexture) const
{
	renderTexture->draw(*mSprite);
}

void Background::Draw(RenderTexture * renderTexture) const
{
	DebugPrintInfo("void Background::Draw(RenderTexture * renderTexture) const");
	renderTexture->draw(*mSprite);
}
