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
#include "Shot.h"

class Player : public Person
{
public:
	Player::Player(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleX, float scaleY, ID3D11ShaderResourceView* shotSpriteSheet) :
		Person(buttonSpriteSheet, positionIn, scaleX, scaleY)
	{
		this->shotSpriteSheet = shotSpriteSheet;
		lastGoodPosition = positionIn;
		skill.reset(new Skill(3, 1, 0, 5, 1));
		over = blockLeft = blockRight = blockTop = blockButtom = stand = false;
		skill->shotSpeed = 12;
	}

	void  Update(float elapsed)
	{
		Person::Update(elapsed);
	}
	
	void  Draw(DirectX::SpriteBatch* batch)
	{
		Person::Draw(batch);
	}

	void  Player::die()
	{
		skill->life -= 1;
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
			reset();
			this->over = false;
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

	void reset()
	{
		//skill.reset(new Skill(startSkill.get));
		Person::reset();
		updateBoundingRect();
	}

	void addBonus(std::shared_ptr<Skill> bonus)
	{
		skill->life += bonus->life;
		skill->point += bonus->point;
		skill->shotDistance += bonus->shotDistance;
		skill->shotSpeed += bonus->shotSpeed;
		skill->speedMove += bonus->speedMove;
	}

	void resize(float scaleX, float scaleY)
	{
		Person::resize(scaleX, scaleY);
	}

public:
	bool												over;
	int													score;
	DirectX::XMFLOAT2									lastGoodPosition;
	std::unique_ptr<Skill>								skill;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	bubbleTexture;
	ID3D11ShaderResourceView*							shotSpriteSheet;
};