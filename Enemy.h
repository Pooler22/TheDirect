#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "InteractiveGameObject.h"
#include "DrawableObject.h"

class Enemy : public InteractiveGameObject
{
public:
	Enemy(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleX, float scaleY, int moveDirectionIn, int point) :
		InteractiveGameObject(buttonSpriteSheet, positionIn, scaleX, scaleY)
	{
		this->moveDirection = moveDirectionIn;
		this->point = point;
	}

	void Enemy::Update(float elapsed)
	{
		if (blockRight || blockLeft)
			moveDirection = -moveDirection;
		move(moveDirection, 0);
		InteractiveGameObject::Update(elapsed);
	}

	void reset()
	{
		InteractiveGameObject::reset();
	}

	void setMoveDirection(int moveDirection)
	{
		this->moveDirection = moveDirection;
	}

	int getMoveDirection()
	{
		return this->moveDirection;
	}

	void setPoint(int point)
	{
		this->point = point;
	}

	int getPoint()
	{
		return this->point;
	}

private:
	int	moveDirection;
	int	point;
};