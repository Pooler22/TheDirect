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

class Player : public Person
{
public:
	Player::Player() :
		Person()
	{
		this->scaleX = 1;
	}

	Player::Player(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleX, float scaleY) :
		Person(buttonSpriteSheet, positionIn, scaleX, scaleY)
	{
		this->scaleX = scaleX;
		shots.reset(new std::vector<Button>());
		skill.reset(new Skill(3, 1, 0, 5, 1));
		over = false;
	}

	void  Draw(DirectX::SpriteBatch* batch)
	{
		for (std::vector<Button>::iterator it = shots->begin(); it != shots->end(); ++it)
		{
			it->Draw(batch);
		}
		animation->Draw(batch, position);
	}

	void  Update(float elapsed)
	{
		if (stand && jumpTime > 0)
			jumpTime--;

		if (!stand)
		{
			move(0, -gravity);
		}
		for (std::vector<Button>::iterator it = shots->begin(); it != shots->end(); ++it)
		{
			it->Update(elapsed);
		}
		animation->Update(elapsed);
	}

	void  Player::die()
	{
		skill->life = skill->life - 1;
		if (skill->life == 0)
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
			resetLevel();
			return true;
		}
		return false;
	}

	int getLife()
	{
		return skill->life;
	}
	void setLife(int life)
	{
		this->skill->life = life;
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
		skill->life = 3;
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
					position.x = position.x + (x * speed * scaleX);
				if (blockDirection == 2 && x < 0)
					position.x = position.x + (x * speed * scaleX);
				position.y = position.y - (y * speed * scaleX);
			}
			else
			{
				position.x = position.x + (x * speed * scaleX);
			}
		}
		else
		{
			position.x = position.x + (x * speed * scaleX);
			position.y = position.y - (y * speed * scaleX);
		}

		updateBoundingRect();
	}

	void fire()
	{
		shots->push_back(Button());
	}

	/*void  Person::addBonus(std::shared_ptr<Bonus_struct> bs)
	{
	score += bs->score;
	speed += bs->speed;
	}*/

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
	int													blockDirection;
	bool												over;

	int													score;
	std::unique_ptr<Skill>								skill;
	std::shared_ptr<std::vector<Button>>				shots;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	bubbleTexture;

};