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
		this->startPosition = positionIn;
		this->scaleX = scaleX;
		this->speed = 10;
		blockLeft = blockRight = blockTop =  blockButtom = stand = false;
	}

	void  Person::Draw(DirectX::SpriteBatch* batch)
	{
		animation->Draw(batch, position);
	}

	void  Person::Update(float elapsed)
	{
		//position.y += force;
		updateBoundingRect();
		animation->Update(elapsed);
	}

	void  move(float x, float y)
	{
		if (blockRight && x > 0)
		{
			x = 0;
		}
		if (blockLeft && x < 0)
		{
			x = 0;
		}
		if (stand && y <= 0)
		{
			force = 0;
			y = 0;
		}
		else 
		{
			if (jumpFlag)
			{
				force = -force;
			}
			force = 5;
		}
		position.x += (x * speed * scaleX);
		position.y -= (y * speed * scaleX) - force * scaleX;

		blockRight = false;
		blockLeft = false;
		stand = false;
		updateBoundingRect();
	}

	void colision(Windows::Foundation::Rect rect, BRICK_BEHAVIOR behavior)
	{
		if (boundingRectangle.IntersectsWith(rect))
			{
			if (boundingRectangle.Right > rect.Left &&
				boundingRectangle.Left < rect.Left - boundingRectangle.Width / 2 &&
				boundingRectangle.Bottom > rect.Top
				)
			{
				blockRight = true;
			}
			if (boundingRectangle.Left < rect.Right &&
				boundingRectangle.Right > rect.Left + boundingRectangle.Width / 2 &&
				boundingRectangle.Bottom > rect.Top
				)
			{
				blockLeft = true;
			}
			if (boundingRectangle.Right - 1 > rect.Left &&
				boundingRectangle.Right + 1 < rect.Right + boundingRectangle.Width &&
				boundingRectangle.Bottom >= rect.Top &&
				boundingRectangle.Top < rect.Top)
			{
				position.y = rect.Y - dimensions.y;
				stand = true;
			}
			if (boundingRectangle.Left + boundingRectangle.Width - 1 > rect.Left &&
				boundingRectangle.Left + boundingRectangle.Width + 1 < rect.Left + rect.Width + boundingRectangle.Width &&
				boundingRectangle.Top <= rect.Bottom&&
				boundingRectangle.Bottom > rect.Bottom)
			{
				position.y = rect.Y + rect.Height;
			}
			updateBoundingRect();
		}
	}
	

	void  Person::setStartPosition()
	{
		position = startPosition;
		updateBoundingRect();
	}

	void jump()
	{
		this->force = this->gravity;
		this->jumpFlag = true;
	}
	
	void resetLevel()
	{
		position = startPosition;
		updateBoundingRect();
	}

public:
	int gravity;
	int force;
	bool jumpFlag;
	bool stand;
	bool blockRight;
	bool blockLeft;
	bool blockTop;
	bool blockButtom;
	float												scaleX;
	int													speed;
	DirectX::XMFLOAT2									startPosition;
	
};