#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"

class Person : public Button
{
public:
	Person::Person()
	{
	}

	Person::Person(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleX, float scaleY) :
		Button(buttonSpriteSheet, positionIn, scaleX, scaleY)
	{
		this->scaleX = scaleX;
		speed = 10;
		gravity = 1;
		stand = false;
		jumpTime = 10;
	}

	virtual void  Person::Draw(DirectX::SpriteBatch* batch)
	{
		animation->Draw(batch, position);
	}

	virtual void  Person::Update(float elapsed)
	{
		if(stand && jumpTime > 0)
			jumpTime--;

		if (!stand)
		{
			move(0, -gravity);
		}
		animation->Update(elapsed);
	}

	virtual void  Person::setStand(bool standIn)
	{
		stand = standIn;
	}

	virtual void  Person::move(float x, float y)
	{
		if (moveDown)
		{
			position.x = position.x + (x * speed * scaleX);
			position.y = position.y - (y * speed * scaleX);
		}
		else
		{
			if (y * speed > 0)
			{
				position.x = position.x + (x * speed * scaleX);
				position.y = position.y - (y * speed * scaleX);
			}
			else
			{
				position.x = position.x + (x * speed * scaleX);
			}
		}
		updateBoundingRect();
	}

	void  Person::jump()
	{
		if(stand && jumpTime > 0)
			move(0, 2);
	}

	

	void  Person::setMoveDown(bool flag)
	{
		moveDown = flag;
	}

	void  Person::setStartPosition()
	{
		position = startPosition;
		updateBoundingRect();
	}
	
	void resetLevel()
	{
		position = startPosition;
		updateBoundingRect();
	}

public:
	float												scaleX;
	int													speed;
	int													jumpTime;
	bool												jumpFlag;
	bool												moveDown;
	bool												stand;
	float												gravity;
	DirectX::XMFLOAT2									startPosition;
	
};