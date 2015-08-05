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

class Brick : public Button
{
public:

	Brick::Brick(ID3D11ShaderResourceView* playerSpriteSheet, DirectX::XMFLOAT2 positionIn, int screenWidth, int screenHeight, XMFLOAT2 sizeIn) : framesOfAnimation(4), framesToBeShownPerSecond(4)
	{
		float rotation = 0.0f;
		float scale = 1.f;

		texture = playerSpriteSheet;
		animation.reset(new AnimatedTexture(DirectX::XMFLOAT2(0.f, 0.f), rotation, scale, 0.0f));
		animation->Load(texture.Get(), framesOfAnimation, framesToBeShownPerSecond);
		
		animation.reset(new AnimatedTexture(DirectX::XMFLOAT2(0.f, 0.f), rotation, (1+textureRectangle.Height/(screenHeight / sizeIn.y)), 0.0f));
		animation->Load(texture.Get(), framesOfAnimation, framesToBeShownPerSecond);

		position = positionIn;
		dimensions.x = textureRectangle.Width = animation->getFrameWidth()/3;
		dimensions.y = textureRectangle.Height = animation->getFrameHeight()/3;
		updateBoundingRect();
		
		behavior = BRICK_BEHAVIOR_NONE;
	}

	void setBehavior(BRICK_BEHAVIOR behaviorIn) 
	{
		behavior = behaviorIn;
	}

	BRICK_BEHAVIOR getBehavior()
	{
		return behavior;
	}

public:
	int						framesOfAnimation;
	int						framesToBeShownPerSecond;
	BRICK_BEHAVIOR			behavior;
};