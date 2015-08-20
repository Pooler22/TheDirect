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
	Enemy(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleX, float scaleY, int moveDirectionIn, int point) :
		Person(buttonSpriteSheet, positionIn, scaleX, scaleY)
	{
		this->moveDirection = moveDirectionIn;
		this->point = point;
	}

	void Update(float elapsed) override
	{
		if (blockRight || blockLeft)
			moveDirection = -moveDirection;
		move(moveDirection, 0);
		Person::Update(elapsed);
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