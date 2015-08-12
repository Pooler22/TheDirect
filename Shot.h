#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Person.h"
#include "Button.h"
#include "Skill.h"

class Shot : public Person
{
public:
	Shot::Shot()
	{}

	Shot::Shot(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleX, float scaleY, bool direction, int speed) :
		Person(buttonSpriteSheet, positionIn, scaleX, scaleY)
	{
		this->speed = speed;
		this->direction = direction;
	}

	bool getDirection()
	{
		return direction;
	}

	void setDirection(bool direction)
	{
		this->direction = direction;
	}

	int getSpeed()
	{
		return speed;
	}

	void setSpeed(int speed)
	{
		this->speed = speed;
	}

	virtual void Update(float elapsed)
	{
		if(direction)
			position.x -= speed * scaleX;
		else
			position.x += speed * scaleX;

		updateBoundingRect();
		animation->Update(elapsed);
	}

	void Draw(DirectX::SpriteBatch* batch)
	{
		animation->Draw(batch, position);
	}


public:
	bool	direction;
	int		speed;
};