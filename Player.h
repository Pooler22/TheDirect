#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Person.h"
#include "Button.h"

class Player : public Person
{
public:
	Player::Player()
	{}

	Player::Player(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleIn ) : framesOfAnimation(4), framesToBeShownPerSecond(4), Person(buttonSpriteSheet, positionIn, scaleIn)
	{
		life = 3;
		score = 0;
		over = false;
	}

	void  Player::die()
	{
		life--; // TODO check value of life
		if (life == 0)
		{
			this->over = true;
		}
		setStartPosition();
	}

	bool Player::gameOver()
	{
		if (this->over)
		{
			this->over = false;
			reset();
			return true;
		}
		return false;
	}
	void  Player::reset()
	{
		life = 3;
	}
	
	int getLife()
	{
		return life;
	}
	void setLife(int life)
	{
		this->life = life;
	}

	int getScore()
	{
		return this->score;
	}

	void setScore(int score)
	{
		this->score = score;
	}
	void resetLevel()
	{
		life = 3;
		score = 0;
		position = startPosition;
		updateBoundingRect();
	}

	virtual void  Player::move(float x, float y)
	{
		if (!moveDown)
		{
			if (y * speed > 0)
			{
				if (blockDirection == 1 && x < 0)
					position.x = position.x + (x * speed);
				if (blockDirection == 2 && x < 0)
					position.x = position.x + (x * speed);
				position.y = position.y - (y * speed);
			}
			else
			{
				position.x = position.x + (x * speed);
			}
		}
		else
		{
			position.x = position.x + (x * speed);
			position.y = position.y - (y * speed);
		}

		updateBoundingRect();
	}

	void setBlockDirection(int direction)
	{
		blockDirection = direction;
		if (direction == 0)
		{

		}
		else if (direction == 1)
		{

		}
		else if (direction == 2)
		{

		}
	}

public:
	int		blockDirection;
	bool	over;
	int		life;
	int		score;
	int		framesOfAnimation;
	int		framesToBeShownPerSecond;

};