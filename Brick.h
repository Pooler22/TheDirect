#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Button.h"

enum BRICK_BEHAVIOR
{
	BRICK_BEHAVIOR_NONE,
	BRICK_BEHAVIOR_BLOCK
};

enum COLISION_TYPE
{
	COLISION_TYPE_TRUE,
	COLISION_TYPE_FALSE,
	COLISION_TYPE_STAND
};

class Brick : public Button
{
public:

	Brick::Brick(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleIn, XMFLOAT2 sizeIn, BRICK_BEHAVIOR behaviorIn) :
		framesOfAnimation(4), 
		framesToBeShownPerSecond(4)
	{
		float rotation = 0.0f;

		scale = scaleIn;
		texture = buttonSpriteSheet;
		animation.reset(new AnimatedTexture(DirectX::XMFLOAT2(0.f, 0.f), rotation, scaleIn, 0.0f));
		animation->Load(texture.Get(), framesOfAnimation, framesToBeShownPerSecond);

		position = positionIn;
		dimensions.x = animation->getFrameWidth();
		dimensions.y = animation->getFrameHeight();		
		behavior = behaviorIn;
		updateBoundingRect();
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
		if (rect.Bottom < boundingRectangle.Top )
		{
			if(boundingRectangle.Contains( Windows::Foundation::Point(rect.Y, rect.Left))||
				boundingRectangle.Contains(Windows::Foundation::Point(rect.Y, rect.Right)))
			{
				if (behavior == BRICK_BEHAVIOR_BLOCK)
				{
					return true;
				}
			}
		}
		else
		{
			return false;
		}
	}

public:

	int					framesOfAnimation;
	int					framesToBeShownPerSecond;
	BRICK_BEHAVIOR		behavior;
};