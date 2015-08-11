#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Button.h"
#include <iostream>

enum COLISION_TYPE
{
	COLISION_TYPE_FALSE = 0x01,
	COLISION_TYPE_TRUE = 0x02,
	COLISION_TYPE_TOP = 0x04,
	COLISION_TYPE_LEFT = 0x08,
	COLISION_TYPE_RIGHT = 0x10,
	COLISION_TYPE_BUTTOM = 0x20
};

enum BRICK_BEHAVIOR
{
	BRICK_BEHAVIOR_NONE,
	BRICK_BEHAVIOR_BLOCK
};

class Brick : public Button
{
public:

	Brick::Brick(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleIn, XMFLOAT2 sizeIn, BRICK_BEHAVIOR behaviorIn) : 
		Button(buttonSpriteSheet, positionIn, scaleIn)
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

	bool isStanding(Windows::Foundation::Rect rect)
	{
		if (this->behavior == BRICK_BEHAVIOR_BLOCK)
		{
			return this->boundingRectangle.IntersectsWith(rect);
		}
		return false;
	}

	unsigned int getColision(Windows::Foundation::Rect rect)
	{
		unsigned int ct;
		
		if (this->behavior == BRICK_BEHAVIOR::BRICK_BEHAVIOR_BLOCK && this->boundingRectangle.IntersectsWith(rect))
		{
			if (this->boundingRectangle.Right > rect.Left +5)
			{
				ct |= COLISION_TYPE::COLISION_TYPE_LEFT;
			}
			else if (this->boundingRectangle.Left < rect.Right - 5)
			{
				ct |= COLISION_TYPE::COLISION_TYPE_RIGHT;
			}
			ct |= COLISION_TYPE::COLISION_TYPE_TRUE;
			return ct;
		}
		else
		{
			return COLISION_TYPE::COLISION_TYPE_FALSE;
		}
	}

public:
	BRICK_BEHAVIOR		behavior;
};