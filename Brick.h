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
	COLISION_TYPE_FALSE = 0x0,
	COLISION_TYPE_TRUE = 0x01,
	COLISION_TYPE_TOP = 0x02,
	COLISION_TYPE_LEFT = 0x04,
	COLISION_TYPE_RIGHT = 0x08,
	COLISION_TYPE_BUTTOM = 0x01
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
		Button(buttonSpriteSheet, positionIn, scaleIn),
		framesToBeShownPerSecond(4),
		framesOfAnimation(4)
	{
		behavior = behaviorIn;
	}

	void setBehavior(BRICK_BEHAVIOR behaviorIn) 
	{
		behavior = behaviorIn;
	}

	BRICK_BEHAVIOR getBehavior()
	{
		return behavior;
	}

	bool isStanding(Windows::Foundation::Rect rect)
	{
		if (behavior == BRICK_BEHAVIOR_BLOCK)
		{
			rect.Y += 1;
			if (boundingRectangle.IntersectsWith(rect))
			{
				return true;
			}
		}
		return false;
	}

	COLISION_TYPE getColision(Windows::Foundation::Rect rect)
	{
		if (boundingRectangle.IntersectsWith(rect))
		{
			if (boundingRectangle.Right <= rect.Left)
			{
				return COLISION_TYPE::COLISION_TYPE_LEFT;
			}
			else if (boundingRectangle.Left >= rect.Right)
			{
				return COLISION_TYPE::COLISION_TYPE_RIGHT;
			}
			return COLISION_TYPE::COLISION_TYPE_TRUE;
		}
		else
		{
			return COLISION_TYPE::COLISION_TYPE_FALSE;
		}
	}

public:

	int					framesOfAnimation;
	int					framesToBeShownPerSecond;
	BRICK_BEHAVIOR		behavior;
};