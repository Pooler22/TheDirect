#pragma once

#include <wrl.h>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Person.h"
#include "Button.h"

typedef struct Bonus_t
{
	int speedMove;
	int point;
	int shotDistance;
	int shotspeed;

	Bonus_t::Bonus_t(int speedMove, int point, int shotDistance, int shotspeed)
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

	Bonus::Bonus(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, float scaleIn, std::shared_ptr<Bonus_t> bonus) :
		Person(buttonSpriteSheet, positionIn, scaleIn),
		framesToBeShownPerSecond(4), 
		framesOfAnimation(4)
	{
		this->bonus = bonus;
	}

public:
	int			framesOfAnimation;
	int			framesToBeShownPerSecond;
	std::shared_ptr<Bonus_t>		bonus;
};