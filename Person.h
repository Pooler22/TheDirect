#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Button.h"

class Person : public Button
{
public:
	Person(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleIn) : framesOfAnimation(4), framesToBeShownPerSecond(4)
	{
		float rotation = 0.f;
		scale = scaleIn;

		texture = buttonSpriteSheet;
		animation.reset(new AnimatedTexture(DirectX::XMFLOAT2(0.f, 0.f), rotation, scaleIn, 0.5f));
		animation->Load(texture.Get(), framesOfAnimation, framesToBeShownPerSecond);
		
		dimensions.x = animation->getFrameWidth();
		dimensions.y = animation->getFrameHeight();
		position.x = positionIn.x;
		position.y = positionIn.y - dimensions.y;
		updateBoundingRect();
		speed = 10;
		gravity = 1;
		stand = false;
		jumpTime = 10;
	}

	void Update(float elapsed)
	{
		if(jumpTime > 0)
			jumpTime--;

		if (!stand)
		{
			move(0, -gravity);
		}
		
		animation->Update(elapsed);
	}

	void setStand(bool standIn)
	{
		stand = standIn;
	}

	void move(float x, float y)
	{
		position.x = position.x + (x * speed);
		position.y = position.y - (y * speed);
		updateBoundingRect();
	}
	void jump() 
	{
		//if(jumpTime > 0)
			//move(0, 2);
	}

public:
	int			jumpTime;

	bool		stand;
	int			speed;
	float		gravity;
	int			framesOfAnimation;
	int			framesToBeShownPerSecond;
};