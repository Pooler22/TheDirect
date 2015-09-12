#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "InteractiveGameObject.h"
#include "DrawableObject.h"
#include "Skill.h"
#include "Shoot.h"

class Player : public InteractiveGameObject
{
public:
	Player::Player(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleX, float scaleY, 
		ID3D11ShaderResourceView* shotSpriteSheet, ID3D11ShaderResourceView* jumpSpriteSheet) :
		InteractiveGameObject(buttonSpriteSheet, positionIn, scaleX, scaleY)
	{
		this->normalSpriteSheet = buttonSpriteSheet;
		this->jumpSpriteSheet = jumpSpriteSheet;
		this->shotSpriteSheet = shotSpriteSheet;
		lastGoodPosition = positionIn;
		skill.reset(new Skill(3, 1, 0, 5, 1));
		over = blockLeft = blockRight = blockTop = blockButtom = stand = false;
		skill->shotSpeed = 12;
	}

	void changeSpriteSheet(ID3D11ShaderResourceView* shotSpriteSheet)
	{
		texture = shotSpriteSheet;
		laodAnimation(scale.x, scale.y);
	}

	void  Player::die()
	{
		skill->life--;
		if (skill->life <= 0)
			this->over = true;
		moveToStartPosition();
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

	void Update(float elapsed)
	{
		InteractiveGameObject::Update(elapsed);
		if (jumpFlagTMP != jumpFlag)
		{
			if(jumpFlag)
			{
				changeSpriteSheet(jumpSpriteSheet.Get());
				jumpFlagTMP = jumpFlag;
			}
			else 
			{
				changeSpriteSheet(normalSpriteSheet.Get());
				jumpFlagTMP = jumpFlag;
			}
		}
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
		InteractiveGameObject::reset();
		updateBoundingRect();
	}

	void addBonus(std::shared_ptr<Skill> bonus)
	{
		skill->operator+=(*bonus);
	}

public:
	bool												jumpFlagTMP;
	bool												over;
	int													score;
	DirectX::XMFLOAT2									lastGoodPosition;
	std::unique_ptr<Skill>								skill;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	normalSpriteSheet;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	bubbleTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	jumpSpriteSheet;
	ID3D11ShaderResourceView*							shotSpriteSheet;
};