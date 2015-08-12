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

	Enemy::Enemy(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleX, float scaleY, int moveDirectionIn) :
		Person(buttonSpriteSheet, positionIn, scaleX, scaleY)
	{
		moveDirection = moveDirectionIn;
	}

	void Update(float elapsed) override
	{
		if (blockRight || blockLeft)
			moveDirection = -moveDirection;

			move(moveDirection, 0);

		animation->Update(elapsed);
	}

public:
	int		moveDirection;
};