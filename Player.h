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
		lastGoodPosition = positionIn;
		this->scaleX = scaleX;
		shots.reset(new std::vector<Button>());
		skill.reset(new Skill(3, 1, 0, 5, 1));
		over = false;
		blockLeft = false;
		blockRight = false;
		blockTop = false;
		blockButtom = false;
		stand = false;
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

	void fire()
	{
		shots->push_back(Button(texture.Get(), position,1.0f, 1.0f));
	}

	void addBonus(std::shared_ptr<Skill> bonus)
	{
		skill->life += bonus->life;
		skill->point += bonus->point;
		skill->shotDistance += bonus->shotDistance;
		skill->shotspeed += bonus->shotspeed;
		skill->speedMove += bonus->speedMove;
	}
public:

	DirectX::XMFLOAT2									lastGoodPosition;
	bool												over;
	int													score;
	std::unique_ptr<Skill>								skill;
	std::shared_ptr<std::vector<Button>>				shots;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	bubbleTexture;

};