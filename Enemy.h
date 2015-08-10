#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Person.h"
#include "Button.h"

class Enemy : public Person
{
public:
	Enemy::Enemy()
	{}

	Enemy::Enemy(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleIn, int moveDirectionIn) : 
		Person(buttonSpriteSheet, positionIn, scaleIn)
	{
		moveDirection = moveDirectionIn;
	}

	void Update(float elapsed) override
	{
		if (stand && jumpTime > 0)
			jumpTime--;

		if (!stand)
		{
			move(0, -gravity);
		}
		if(moveDirection == 1)
		{
			move(1,0);
		}
		else
		{
			move(-1, 0);
		}
		animation->Update(elapsed);
	}

public:
	int		moveDirection;
};