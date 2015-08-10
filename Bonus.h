#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Person.h"
#include "Button.h"

typedef struct SKILL_T
{
	int speedMove;
	int point;
	int shotDistance;
	int shotspeed;

	SKILL_T::SKILL_T()
	{
		this->speedMove = this->point = this->shotDistance = this->shotspeed = 0;
	}

	SKILL_T::SKILL_T(int speedMove, int point, int shotDistance, int shotspeed)
	{
		this->speedMove = speedMove;
		this->point = point;
		this->shotDistance = shotDistance;
		this->shotspeed = shotspeed;
	}
};

class Bonus : public Person
{
public:
	Bonus::Bonus()
	{}

	Bonus::Bonus(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleIn, std::shared_ptr<SKILL_T> bonus) :
		Person(buttonSpriteSheet, positionIn, scaleIn)
	{
		this->bonus = bonus;
	}

public:
	std::shared_ptr<SKILL_T>		bonus;
};