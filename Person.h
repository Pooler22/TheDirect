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
	Person::Person(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleX, float scaleY) :
		Button(buttonSpriteSheet, positionIn, scaleX, scaleY)
	{
		this->startPosition = positionIn;
		this->direction = blockLeft = blockRight = blockTop =  blockButtom = stand = false;
		this->speed = 10;
	}

	void  Person::Draw(DirectX::SpriteBatch* batch)
	{
		if (direction)
		{
			animation->Draw(batch, position, DirectX::SpriteEffects::SpriteEffects_FlipHorizontally);
		}
		else 
		{
			animation->Draw(batch, position);
		}
		
	}

	void  Person::Update(float elapsed)
	{
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

		if (x > 0)
		{
			direction = false;
		}
		else if (x < 0)
		{
			direction = true;
		}

		position.x += (x * speed * scaleX);
		position.y -= (y * speed * scaleX) - force * scaleX;

		blockRight = false;
		blockLeft = false;
		stand = false;
		updateBoundingRect();
	}

	void correctPersonPosition(float screenWidth, float screenHeight)
	{
		if (position.y > screenHeight)
		{
			position.y = 0.0 - dimensions.y;
		}
		else if (position.y < 0.0 - dimensions.y)
		{
			position.y = screenHeight;
		}
		if (getPosition().x > screenWidth)
		{
			position.x = 0.0 - dimensions.x;
		}
		else if (getPosition().x < 0.0 - dimensions.x)
		{
			position.x = screenWidth;
		}
		updateBoundingRect();
	}

	void colision(Windows::Foundation::Rect rect)
	{
		if (boundingRectangle.Top + 1 <= rect.Bottom &&
			boundingRectangle.Bottom - 1 >= rect.Top &&
			boundingRectangle.Right + 1 >= rect.Left &&
			boundingRectangle.Left < rect.Left
			)
		{
			position.x = rect.X - dimensions.x;
			blockRight = true;
		}
		if (boundingRectangle.Top + 1 <= rect.Bottom &&
			boundingRectangle.Bottom - 1 >= rect.Top &&
			boundingRectangle.Left - 1 <= rect.Right &&
			boundingRectangle.Right > rect.Right
			)
		{
			position.x = rect.X + rect.Width;
			blockLeft = true;
		}
		if (boundingRectangle.Right - 1 > rect.Left &&
			boundingRectangle.Left + 1 < rect.Right &&
			boundingRectangle.Bottom >= rect.Top &&
			boundingRectangle.Top < rect.Top)
		{
			position.y = rect.Y - dimensions.y;
			stand = true;
		}
		if (boundingRectangle.Right - 1 > rect.Left &&
			boundingRectangle.Left + 1 < rect.Right &&
			boundingRectangle.Top <= rect.Bottom &&
			boundingRectangle.Bottom > rect.Bottom)
		{
			position.y = rect.Y + rect.Height;
		}
		updateBoundingRect();
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
	bool direction; //0 lat, 1 right
	int gravity;
	int force;
	bool jumpFlag;
	bool stand;
	bool blockRight;
	bool blockLeft;
	bool blockTop;
	bool blockButtom;
	int													speed;
	DirectX::XMFLOAT2									startPosition;
	
};