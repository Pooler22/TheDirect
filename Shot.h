#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Person.h"
#include "DrawableObject.h"
#include "Skill.h"

class Shot : public Person
{
public:
	Shot::Shot(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleX, float scaleY, bool direction, int speed) :
		Person(buttonSpriteSheet, positionIn, scaleX, scaleY)
	{
		this->speed = 6;
		this->direction = direction;
	}

	void Update(float elapsed)
	{
		if (blockLeft || blockRight)
		{
			position.y -= (speed * scale.y);
		}
		else if (direction)
			position.x -= speed * scale.x;
		else
			position.x += speed * scale.x;

		updateBoundingRect();
		animation->Update(elapsed);
		//left = right = stand = blockRight = blockTop = blockRight = blockLeft = false;
		DrawableObject::Update(elapsed);
	}

	bool getDirection()
	{
		return this->direction;
	}

	void setDirection(bool direction)
	{
		this->direction = direction;
	}

	int getSpeed()
	{
		return this->speed;
	}

	void setSpeed(int speed)
	{
		this->speed = speed;
	}

private:
	bool direction;
	int	speed;
};