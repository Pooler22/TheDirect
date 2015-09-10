#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "DrawableObject.h"
#include <iostream>

enum BRICK_BEHAVIOR
{
	BRICK_BEHAVIOR_NONE,
	BRICK_BEHAVIOR_BLOCK
};

class Brick : public DrawableObject
{
public:
	Brick::Brick(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleX, float scaleY, XMFLOAT2 sizeIn, BRICK_BEHAVIOR behaviorIn) :
		DrawableObject(buttonSpriteSheet, positionIn, scaleX, scaleY)
	{
		this->behavior = behaviorIn;
	}

	void setBehavior(BRICK_BEHAVIOR behaviorIn) 
	{
		this->behavior = behaviorIn;
	}

	BRICK_BEHAVIOR getBehavior()
	{
		return this->behavior;
	}

private:
	BRICK_BEHAVIOR behavior;
};