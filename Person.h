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
		this->direction = blockLeft = blockRight = blockTop = blockButtom = stand = false;
		this->speed = 8;
		x = 1;
		y = 1;
		force = 8;
		left = right = jumpFlag = blockTop = blockRight = blockLeft = false;
	}

	void  Person::Draw(DirectX::SpriteBatch* batch)
	{
		updateBoundingRect();
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
		if (right && !blockRight)
			position.x += (speed * scale.x);
		else if (left && !blockLeft)
			position.x -= (speed * scale.x);
		if (jumpFlag)
		{
			position.y += (0 * speed * scale.y) - force * scale.y;
			force--;
			stand = false;
			if (force == 8)
			{
				jumpFlag = false;
			}
		}
		else if (stand) {}
		else if (!stand)
			position.y += force * scale.y;

		updateBoundingRect();
		animation->Update(elapsed);
		left = right = stand = blockRight = blockTop = blockRight = blockLeft = false;
	}

	void  move(float x, float y)
	{
		if (x > 0)
			right = true;
		else if (x < 0)
			left = true;
		if (!jumpFlag && y > 0 && stand)
		{
			jumpFlag = true;
			force = gravity;
		}

		if (x > 0)
		{
			direction = false;
		}
		else if (x < 0)
		{
			direction = true;
		}
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
		if (boundingRectangle.Right > rect.Left &&
			boundingRectangle.Left < rect.Right &&
			boundingRectangle.Bottom >= rect.Top &&
			boundingRectangle.Top < rect.Top)
		{
			position.y = rect.Y - dimensions.y;
			updateBoundingRect();
			stand = true;
		}
		if (boundingRectangle.Bottom <= rect.Bottom + 1 &&
			boundingRectangle.Bottom > rect.Top + 1 &&
			boundingRectangle.Right >= rect.Left &&
			boundingRectangle.Left < rect.Left
			)
		{
			position.x = rect.X - dimensions.x;
			updateBoundingRect();
			blockRight = true;
			direction = true;
		}
		if (boundingRectangle.Bottom <= rect.Bottom + 1 &&
			boundingRectangle.Bottom > rect.Top + 1 &&
			boundingRectangle.Left <= rect.Right &&
			boundingRectangle.Right > rect.Right
			)
		{
			position.x = rect.X + rect.Width;
			updateBoundingRect();
			blockLeft = true;
			direction = false;
		}

	}


	void  Person::setStartPosition()
	{
		position = startPosition;
		updateBoundingRect();
	}

	void setStartPositionExt(DirectX::XMINT2 position)
	{
		this->position = this->startPosition = DirectX::XMFLOAT2(position.x, position.y);
		setStartPosition();
	}

	void jump()
	{
		this->force = this->gravity;
		this->jumpFlag = true;
	}

	void reset()
	{
		position = startPosition;
		updateBoundingRect();
	}

public:
	bool right;
	bool left;
	int x;
	int y;
	bool direction; //0 lat, 1 right
	int gravity = 15;
	int force;
	bool jumpFlag;
	bool stand;
	bool blockRight;
	bool blockLeft;
	bool blockTop;
	bool blockButtom;
	int	speed;
	DirectX::XMFLOAT2	startPosition;

};